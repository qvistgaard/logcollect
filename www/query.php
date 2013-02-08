<?php
if(isset($_GET['query'])){
	$query = '../src/search -i ../var/messages -q "'.trim($_GET['query']).'" -f logline';
	$query = trim(`$query`);

	header('Content-Type: application/json');
	echo $query;
}
?>