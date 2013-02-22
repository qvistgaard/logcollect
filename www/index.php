<html>
	<head>
		<title></title>
		<script type="text/javascript" src="js/prototype.js"></script>
		<script type="text/javascript" src="js/search.js"></script>

		<script type="text/javascript" src="js/flotr2/flotr2.min.js"></script>


		<style>
			body {
				font-family: Verdana;
				font-size: 8pt;
			}

			small {
				font-size: 7pt;
				line-height: 20px;

			}

			form {
				margin: 0px;
			}

			h2 {
				font-size: 13pt;
			}

			h3 {
				font-size: 11pt;
			}

			.logline {
				font-family: courier, monospace;
				font-size: 9pt;
				line-height: 20px;
			}

			.box {
				background-color: #DDEEFF;
				border: 1px solid #CCDDEE;
				margin: 2px;
				padding: 5px;
				margin-bottom: 10px;
				font-size: 8pt;
			}

			.box h3 {
				margin: 0px;
				margin-bottom: 7px;
			}

			.box ul  {
				list-style: none;
				margin-bottom: 20px;
				margin-top: 0px;
				padding: 0px;
			}

			.box ul li {
				padding: 0px;
				margin-left: 0px;
				margin-bottom: 5px;
			}

			.result {
				border-top: 1px solid #CCDDEE;
				border-bottom: 1px solid #CCDDEE;
				margin-top: 10px;
				margin-bottom: 10px;
				padding-top: 10px;
				padding-bottom: 15px;
			}

			.highlight {
				background-color: #afff98;
			}

			.loader {
				background-image: url('images/preloader.gif');
				background-repeat: no-repeat;
				background-size: 14px 14px;
				background-position: 99.4% center;
			}

			#pager ul {
				margin: 0px;
				padding: 0px;
			}


			#pager ul li {
				float: left;
				list-style: none;
				margin-right:5px;
			}

			table td {
				vertical-align: top;
			}

			.current {
				font-weight:bold;
			}


			.result-item {
				padding: 5px;
			}

			.result-item:nth-child(even) {
				background-color: #F3f9ff;
			}

			.result-item small {
				color: #888888;
			}
			.result-item .field-link {
				font-weight: bold;
				margin-right: 10px;
			}

			#chart {
				width: 96%;
				height: 200px;
				margin-top: 10px;
				margin-bottom: 10px;
			}

		</style>
	</head>
	<body>
		&#160;&#160;Logcollectd
		<table border="0" style="width: 100%;">
			<tbody>
				<tr>
					<td style="width: 250px;">
						<div class="box">
							<h2>Filter results</h2>

							<h3>Fields:</h3>
							<ul id="result-fields">
								<li>pid: 1232 (12)</li>
								<li>logsource: mx1.zhosting.dk (234)</li>
								<li>program: postfix/smtpd (34)</li>
								<li>timestamp: 2011-11-09 (99)</li>
							</ul>

							<h3>Search History:</h3>
							<ul id="history"></ul>
						</div>
					</td>
					<td>
						<div class="box">
							<form method="get" onsubmit="Search.q($('query').value); return false;">
								<table cellpadding="0" cellspacing="0" style="width: 100%;">
									<tbody>
										<tr>
											<td><input type="text" name="query" id="query" autofocus="true" style="border: 1px solid #BBCCDD; width:100%;"/></td>
											<td style="width: 10px;"><input type="submit" value="S"/></td>
										</tr>
									</tbody>
								</table>
							</form>
						</div>
						<div id="result-container" style="display: none;">
							<span id="search-range">1 - 20</span> of <span id="search-result">453534</span> results for <strong id="search-query">SEARCH QUERY</strong> -
							<span id="search-total">123.412.313.213</span> documents searched in 0.72592711448669 seconds.
							<div id="chart"></div>
							<div class="result" id="result-list">
								<!--
								<div style="padding: 5px; background-color: #F3f9ff;">
									<div class="logline">2011-11-09T18:00:06+01:00 mx1.zhosting.dk postfix/smtpd[71327]: NOQUEUE: reject: RCPT from unknown[14.194.91.24]: 554 5.7.1 Service unavailable; Client host [14.194.91.24] blocked using zen.spamhaus.org; http://www.spamhaus.org/query/bl?ip=14.194.91.24; from=&lt;alvaro@incoesa.com&gt; to= proto=ESMTP helo= </div>
									<small style="color: #888888;">pid: <a href="?query=%2Btimestamp%3A%5B2011110919+TO+2011110920%5D+%2Brom%3Aalvaro%40incoesa.com+%2Bpid%3A71327"><strong>71327</strong></a></small>&#160;
									<small style="color: #888888;">o: <a href="?query=%2Btimestamp%3A%5B2011110919+TO+2011110920%5D+%2Brom%3Aalvaro%40incoesa.com+%2Bo%3Aangeliquericheykw%40standby.dk"><strong>angeliquericheykw@standby.dk</strong></a></small>&#160;
									<small style="color: #888888;">elo: <a href="?query=%2Btimestamp%3A%5B2011110919+TO+2011110920%5D+%2Brom%3Aalvaro%40incoesa.com+%2Belo%3Aa-121f11198ab64"><strong>a-121f11198ab64</strong></a></small>&#160;
									<small style="color: #888888;">roto: <a href="?query=%2Btimestamp%3A%5B2011110919+TO+2011110920%5D+%2Brom%3Aalvaro%40incoesa.com+%2Broto%3AESMTP"><strong>ESMTP</strong></a></small>&#160;
									<small style="color: #888888;">logsource: <a href="?query=%2Btimestamp%3A%5B2011110919+TO+2011110920%5D+%2Brom%3Aalvaro%40incoesa.com+%2Blogsource%3Amx1.zhosting.dk"><strong>mx1.zhosting.dk</strong></a></small>&#160;
									<small style="color: #888888;">program: <a href="?query=%2Btimestamp%3A%5B2011110919+TO+2011110920%5D+%2Brom%3Aalvaro%40incoesa.com+%2Bprogram%3Apostfix%2Fsmtpd"><strong>postfix/smtpd</strong></a></small>&#160;
									<small style="color: #888888;">rom: <a href="?query=%2Btimestamp%3A%5B2011110919+TO+2011110920%5D+%2Brom%3Aalvaro%40incoesa.com+%2Brom%3Aalvaro%40incoesa.com"><strong><span style="background-color: #DDFFCC; font-weight: bold;">alvaro@incoesa.com</span></strong></a></small>&#160;
									<small>timestamp: <strong>2011-11-09T20:00:06+0100</strong></small>&#160;<small style="color: #888888;">ttp://www.spamhaus.org/query/bl?ip: <a href="?query=%2Btimestamp%3A%5B2011110919+TO+2011110920%5D+%2Brom%3Aalvaro%40incoesa.com+%2Bttp%3A%2F%2Fwww.spamhaus.org%2Fquery%2Fbl%3Fip%3A14.194.91.24"><strong>14.194.91.24</strong></a></small>&#160;
								</div>
								-->

							</div>
						</div>
<!--
						<div id="pager">
							<ul>
								<li>Prev</li>
								<li>1</li>
								<li>2</li>
								<li>3</li>
								<li>4</li>
								<li>Next</li>
							</ul>
						</div>
						-->
					</td>
				</tr>
			</tbody>
		</table>

<!--
		<script>

			(function basic_time(container) {

				var
						d1 = [],
						start = new Date("2009/01/01 01:00").getTime(),
						options, graph, i, x, o;
				for (i = 0; i < 100; i++) {
					x = start + (i * 1000 * 3600 * 24 * 36.5);

					d1.push([x, i + Math.random() * 30 + Math.sin(i / 20 + Math.random() * 2) * 20 + Math.sin(i / 10 + Math.random()) * 10]);
				}

				options = {
					bars: {
						show: true,
						horizontal: false,
						shadowSize: 0,
						barWidth: 0.5
					},
					xaxis: {
						mode: 'time',
						labelsAngle: 45
					},
					selection: {
						mode: 'x'
					},
					HtmlText: false,
					title: 'Time'
				};

				// Draw graph with default options, overwriting with passed options


				function drawGraph(opts) {

					// Clone the options, so the 'options' variable always keeps intact.
					o = Flotr._.extend(Flotr._.clone(options), opts || {});

					// Return a new graph.
					return Flotr.draw(
							container, [d1], o);
				}

				graph = drawGraph();

				Flotr.EventAdapter.observe(container, 'flotr:select', function(area) {
					// Draw selected area
					graph = drawGraph({
						xaxis: {
							min: area.x1,
							max: area.x2,
							mode: 'time',
							labelsAngle: 45
						},
						yaxis: {
							min: area.y1,
							max: area.y2
						}
					});
				});

				// When graph is clicked, draw the graph with default area.
				Flotr.EventAdapter.observe(container, 'flotr:click', function() {
					graph = drawGraph();
				});
			})(document.getElementById("chart"));

		</script>
-->

	</body>
</html>