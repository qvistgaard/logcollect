var Search = {
	history: new Array(),
	graph: null,
	chart: {},
	dates: new Array(),
	cancel: false,
	count: 0,
	selected: null,
	flotr_opt: {
		bars: {
			show: true,
			horizontal: false,
			shadowSize: 0,
//			barWidth: 1,
			lineWidth:0.5
		},
		xaxis: {
//			mode: 'time',
//			tickUnit: 'year',
//			timeUnit: 'day',
			noTicks: 6,
			// labelsAngle: 45,
			tickFormatter: function(x) {
				x = parseInt(x);
				if(Search.dates[x]){
					x = new Date(Search.dates[x]);
					return x.getFullYear()+'-'+ x.getDate()+ ' '+x.getHours()+':'+ x.getMinutes();
				}
				return '';
			}
		},
		yaxis: {
			min: 0
		},
		mouse: {
			track: true,
			relative: true,
			trackFormatter: function(mark){
				x = parseInt(mark.y);
				return " " + x + " events";
			},
			trackClick: true
		},
		selection: {
			mode: 'x'
		},
		HtmlText: false
	},

	q: function(q, offset, limit, callback){
		if(!offset){
			offset = 0;
		}
		if(!limit){
			limit = 30;
		}
		if(!callback){
			callback = Search._query;
		}
		$('query').addClassName('loader');
		new Ajax.Request('query.php', {
			parameters: { query: q, o: offset, l: limit },
			method: 'GET',
			onSuccess: callback
		});
	},

	highlight: function(q, string){
		q = q.split(' ');
		for(var i = 0; i < q.length; i++){
			q[i] = q[i].replace(/^(\+|-)/, '');
			q[i] = q[i].replace(/^.*?:/, '');
			q[i] = q[i].replace(/\*.*$/, '');
			string = string.replace(q[i], '<span class="highlight">'+q[i]+'</span>');
		}
		return string;
	},

	_query: function(response) {
		Search.cancel = true;
		Search.count = 0;
		$('result-container').show();
		Search.chart = {};

		var query = response.request.options.parameters.query;
		var offset = response.request.options.parameters.o;
		var limit = response.request.options.parameters.l;

		if(Search.history.indexOf(query) == -1){
			Search.history.push(query);
			if(Search.history.length > 20){
				Search.history.shift();
			}
		}

		var count = response.responseJSON.count;
		$('query').value = query;

		$('search-total').update(response.responseJSON.total);
		$('search-result').update(count);
		$('search-query').update(query);

		var start = (response.responseJSON.offset * response.responseJSON.limit) + 1
		var end = (response.responseJSON.offset + 1) * response.responseJSON.limit;
		if(end > count){
			end = count;
		}

		$('search-range').update(start+' - '+end);
		$('result-list').update('');

		var result = $('result-list');
		var fields = new Array();

		for(var i = 0; i < response.responseJSON.result.length; i++){
			var e = document.createElement('div');
			e.setAttribute('class', 'result-item');

			var logline = e.appendChild(document.createElement('div'));
			logline.setAttribute('class', 'logline');
			logline.appendChild(document.createTextNode( response.responseJSON.result[i].logline));
			Element.update(logline, Search.highlight(query, response.responseJSON.result[i].logline));

			for (field in response.responseJSON.result[i]){
				if(field != 'logline'){
					var logfield = e.appendChild(document.createElement('small'));
					logfield.appendChild(document.createTextNode(field+': '));

					var a = logfield.appendChild(document.createElement('a'));
					a.setAttribute('class', 'field-link');
					a.setAttribute('href', 'javascript:void(0)');
					if(field == '_timestamp'){
						timestamp = new Date(response.responseJSON.result[i][field] * 1000);
						response.responseJSON.result[i][field] = timestamp;
						Search._chartData(timestamp, 1);
					}
					a.appendChild(document.createTextNode(response.responseJSON.result[i][field]));
					// Element(a).update();
					Element.observe(a, 'click', Search.q.bind(this, query+' +'+field+':'+response.responseJSON.result[i][field]));

					if(fields.indexOf(field) == -1){
						fields.push(field);
					}
				}
			}
			result.appendChild(e);
		}

		Search.cancel = false;



		$('result-fields').update('');
		for(var i = 0; i < fields.length; i++){
			var li = $('result-fields').appendChild(document.createElement('li'))
			li.appendChild(document.createTextNode(fields[i]));
		}

		$('history').update('');

		for(var i = Search.history.length -1; i >= 0; i--){
			var li = document.createElement('li');

			var num = (i - Search.history.length)*-1;

			var a = li.appendChild(document.createElement('a'));
			a.setAttribute('href', 'javascript:void(0)');
			if(query == Search.history[i]){
				a.setAttribute('class', 'current');
			}
			a.appendChild(document.createTextNode(Search.history[i]));
			Element.observe(a, 'click', Search.q.bind(this, Search.history[i], offset, limit, Search._query));

			$('history').appendChild(li);
		}

		Search._drawChart();

		if((limit+offset) < response.responseJSON.count){
			Search.q(query, limit+offset, 2500, Search._chart);
		} else {
			$('query').removeClassName('loader');
		}
	},

	_chart: function(response){
		var query = response.request.options.parameters.query;
		var offset = response.request.options.parameters.o;
		var limit = response.request.options.parameters.l;

		$('search-total').update(response.responseJSON.total);

		for(var i = 0; i < response.responseJSON.result.length; i++){
			if(response.responseJSON.result[i]._timestamp){
				timestamp = new Date(response.responseJSON.result[i]._timestamp * 1000);
				Search._chartData(timestamp, 1);
			}
		}
		Search._drawChart();
		// $('query').removelassName('loader');
		if(Search.cancel != true && (limit+offset) < (response.responseJSON.count + limit)){
			Search.q(query, limit+offset, limit, Search._chart);
		} else {
			$('query').removeClassName('loader');
		}
	},

	_chartData: function(date, value){
		diff = new Date(date);
		diff.setMilliseconds(0);

		date = diff.getTime();
		if(!Search.chart[date]){
			Search.chart[date] = 0;
			Search.count++;
		}
		Search.chart[date] += value;
	},

	_drawChart: function(){
		Search.dates = new Array();
		for(key in Search.chart){
			Search.dates.push(parseInt(key));
		}
		Search.dates.sort();

		nchart = {};
		ndates = new Array();
		diff = Math.ceil((Search.dates[Search.dates.length - 1] - Search.dates[0]) / 100);

		for(var i = 0; i < Search.dates.length; i++){
			var tdate = Search.dates[i] - (Search.dates[i] % diff);
			if(!nchart[tdate]){
				nchart[tdate] = 0;
				ndates.push(tdate);
			}
			nchart[tdate] += Search.chart[Search.dates[i]];

		}
		Search.dates = ndates;
		Search.chart = nchart;


		var data = new Array();
		for(var i = 0; i < Search.dates.length; i++){
			data.push([ i, Search.chart[Search.dates[i]] ]);
		}
		Search.graph = Flotr.draw($("chart"), [ data ], Search.flotr_opt);
	},

	_makeDate: function(timestamp){
		month = String("00" + (timestamp.getMonth()+1)).slice(-2);
		date = String("00" + timestamp.getDate()).slice(-2);
		hour = String("00" + timestamp.getHours()).slice(-2);
		minute = String("00" + timestamp.getMinutes()).slice(-2);
		second = String("00" + timestamp.getSeconds()).slice(-2);

		return timestamp.getFullYear()+month+date+hour+minute+second;
	},

	_searchClickBar: function(position){
		if(position.hit){
			console.log(position.hit.index);
			var hit = position.hit.index;


			var start = new Date(Search.dates[hit-1]);
			var end = new Date(Search.dates[hit+2]);

			start = Search._makeDate(start);
			end = Search._makeDate(end);

			var q = 'timestamp:['+start+' TO '+end+']';

			$('query').value = $('query').value.replace(/\s+\+timestamp.*?\]/, '');
			$('query').value += ' +'+q;
			Search.q($('query').value);
		}
	}
};

Element.observe(document, 'dom:loaded', function(){
	// Flotr.draw($("chart"), [ ], Search.flotr_opt);
	Flotr.EventAdapter.observe($("chart"), 'flotr:select', function(area) {
		area.x1 = Math.floor(area.x1);
		if(area.x1 < 0){
			area.x1 = 0;
		}
		area.x2 = Math.ceil(area.x2);
		if(area.x2 > (Search.dates.length-1)){
			area.x2 = ((Search.dates.length-1));
		}

		var start = new Date(Search.dates[area.x1]);
		var end = new Date(Search.dates[area.x2]);

		start = Search._makeDate(start);
		end = Search._makeDate(end);

		var q = '_timestamp:['+start+' TO '+end+']';

		$('query').value = $('query').value.replace(/\s+\+_timestamp.*?\]/, '');
		$('query').value += ' +'+q;
		Search.q($('query').value);
	});

	Flotr.EventAdapter.observe($("chart"), 'flotr:click', function(position) {
		window.setTimeout(Search._searchClickBar.bind(this, position), 200);
	});



/*
	Flotr.EventAdapter.observe($("chart"), 'flotr:click', function(position) {
		console.log(position, position.hit);


/*
		for(key in position){
			console.log(key);
		}

		if(position.hit){
			var hit = position.Hit.x;

			console.log(Search.dates[hit], Search.dates[hit+1])


		}



//		hit
*/
//	});
//	console.log('TEST');
});