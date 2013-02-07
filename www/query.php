<form>
	<input type="text" name="query" value="<?php echo $_GET['query']; ?>" size="200"/>
	<input type="submit"/>
</form>
<?php if(isset($_GET['query'])){ ?>
<?php

	$time_start = microtime(true);
$query = '../src/search -i ../var/ -q "'.$_GET['query'].'" -f logline';



$query = trim(`$query`);
echo '<pre>';
// echo '<pre>'.$query;

$result = json_decode($query);
if(is_null($result)){
	echo $query;
}

$sterms = explode(" ", $_GET['query']);
foreach($sterms as &$sterm){
	$sterm = preg_replace('/^.*?:/', '', $sterm);
	$sterm = str_replace('+', '', $sterm);
	$sterm = preg_quote($sterm, '/');
}
	//echo var_dump($query);

function hstring(array $terms, $string){
	$string = preg_replace('/('.implode('|', $terms).')/i', '<span style="background-color: #CDCD00">\\1</span>', $string);
	/*
	foreach($terms as $term){
	//	var_dump($term);


		$string = preg_replace('/('.implode('|', $terms).')/i', '<span style="background-color: #CDCD00">\\1</span>', $string);
	}
	*/
	return $string;
}

?>
	<table>
		<tbody>
			<?php while(list($key, $val) = each($result->result)){ ?>
			<tr>
				<td><pre><?php
					echo hstring($sterms, $val->logline).'<br/>';
					foreach($val as  $key => $val){
						if($key != 'logline'){
							echo '<a href="?query='.$_GET['query'].' '.$key.':'.$val.'"><small>'.$key.': <strong>'.hstring($sterms, $val).'</strong></small></a>&#160;';
						}
					}

				?></pre></td>
			</tr>
			<?php } ?>
		</tbody>
	</table>

<?php } ?>

<?php


$time_end = microtime(true);
$time = $time_end - $time_start;

echo "search finished in $time seconds\n";
?>