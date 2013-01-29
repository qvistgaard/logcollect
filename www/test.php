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

date_default_timezone_set('Europe/Copenhagen');

$sterms = explode(" ", $_GET['query']);
foreach($sterms as &$sterm){
	$sterm = preg_replace('/^.*?:/', '', $sterm);
	$sterm = str_replace('+', '', $sterm);
	$sterm = preg_quote($sterm, '/');
}
	//echo var_dump($query);

function hstring(array $terms, $string){
	$string = preg_replace('/('.implode('|', $terms).')/i', '<span style="background-color: #DDFFCC; font-weight: bold;">\\1</span>', $string);
	/*
	foreach($terms as $term){
	//	var_dump($term);


		$string = preg_replace('/('.implode('|', $terms).')/i', '<span style="background-color: #CDCD00">\\1</span>', $string);
	}
	*/
	return $string;
}
$i = 0;

echo 'Total number of hits: '.number_format($result->count).' - ';

$time_end = microtime(true);
$time = $time_end - $time_start;
echo "search finished in $time seconds";

?>

	<table>
		<tbody>
			<?php while(list($key, $val) = each($result->result)){ $i++; ?>
			<tr style="<?php if($i % 2 == 1){ echo 'background-color: #EAEAEA;'; } ?>">
				<td style="padding: 5px;"><?php


					echo hstring($sterms, $val->logline).'<br/>';
					foreach($val as  $key => $val){
						if($key == 'timestamp'){
							// strftime('%FT%T%z', $val)

							echo '<small>'.$key.': <strong>'.strftime('%FT%T%z', $val).'</strong></small>&#160;';
						} else if($key != 'logline'){
							echo '<small style="color: #888888;">'.$key.': <a href="?query='.urlencode($_GET['query'].' +'.$key.':'.$val).'"><strong>'.hstring($sterms, $val).'</strong></a></small>&#160;';
						}
					}

				?></td>
			</tr>
			<?php } ?>
		</tbody>
	</table>

<?php } ?>

<?php



?>