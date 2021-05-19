<?php

$conexion = pg_connect("host=ec2-107-20-153-39.compute-1.amazonaws.com port=5432 dbname=d8r3vjhhkehuv4 user=ybklwjsgmubonm password=9fd44fba109201c501e9ee0bac95f99c73b66dca9f13f0a45c0949f5b0ed9b8a");

$limiteDB = 60;
$datosSalvados = 20;

$query = "SELECT * FROM plazas ORDER BY id DESC";
$consulta = pg_query($conexion, $query);
$filas = pg_num_rows($consulta);
$respuesta=pg_fetch_all($consulta);
$id=$respuesta[0]["id"];

$index = $id - datosSalvados;

if($filas>limiteDB){
	$query2 = 'DELETE FROM '. $tabla .' WHERE id <='.$index;
	$consulta2 = pg_query($conexion, $query2);
}


