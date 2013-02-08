var Search = {
	history: new Array(),

	q: function(q){
		new Ajax.Request('query.php', {
			parameters: { query: q  },
			method: 'GET',
			onSuccess: function(response) {

				var query = response.request.options.parameters.query;
				if(Search.history.indexOf(query) == -1){
					Search.history.push(query);
					if(Search.history.length > 20){
						Search.history.shift();
					}
				}

				var count = response.responseJSON.result.length;
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

				for(var i = 0; i < count; i++){
					var e = document.createElement('div');
					e.setAttribute('class', 'result-item');

					var logline = e.appendChild(document.createElement('div'));
					logline.setAttribute('class', 'logline');
					logline.appendChild(document.createTextNode(response.responseJSON.result[i].logline));

					for (field in response.responseJSON.result[i]){
						if(field != 'logline'){
							var logfield = e.appendChild(document.createElement('small'));
							logfield.appendChild(document.createTextNode(field+': '));

							var a = logfield.appendChild(document.createElement('a'));
							a.setAttribute('class', 'field-link');
							a.setAttribute('href', 'javascript:void(0)');
							a.appendChild(document.createTextNode(response.responseJSON.result[i][field]));

							Element.observe(a, 'click', Search.q.bind(this, query+' +'+field+':'+response.responseJSON.result[i][field]));

							if(fields.indexOf(field) == -1){
								fields.push(field);
							}

						}
					}
					result.appendChild(e);
				}

				$('result-fields').update('');
				for(var i = 0; i < fields.length; i++){
					var li = $('result-fields').appendChild(document.createElement('li'))
					li.appendChild(document.createTextNode(fields[i]));
				}

				$('history').update('');

				for(var i = Search.history.length -1; i >= 0; i--){
					console.log(i, Search.history.length);
					var li = document.createElement('li');

					var num = (i - Search.history.length)*-1;

					var a = li.appendChild(document.createElement('a'));
					a.setAttribute('href', 'javascript:void(0)');
					if(query == Search.history[i]){
						a.setAttribute('class', 'current');
					}
					a.appendChild(document.createTextNode(Search.history[i]));
					Element.observe(a, 'click', Search.q.bind(this, Search.history[i]));

					$('history').appendChild(li);
				}
			}
		});
	}
};