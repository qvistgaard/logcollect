<?php
if(isset($_GET['query'])){

	$limit = 30;
	$offset = 0;

	if(isset($_GET['l']) && $_GET['l'] == (int) $_GET['l']){
		$limit = (int) $_GET['l'];
	}
	if(isset($_GET['o']) && $_GET['o'] == (int) $_GET['o']){
		$offset = (int) $_GET['o'];
	}

	$query = '../src/search -i ../var/messages -q "'.trim($_GET['query']).'" -f logline -l '.(int) $limit.' -o '.(int) $offset.'';
	$query = trim(`$query`);

	header('Content-Type: application/json');
	echo $query;
}
?>