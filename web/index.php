<?php

use Symfony\Component\HttpFoundation\Request;
date_default_timezone_set('America/Bogota');

require('../vendor/autoload.php');

$app = new Silex\Application();
$app['debug'] = true;

// Register the monolog logging service
$app->register(new Silex\Provider\MonologServiceProvider(), array(
  'monolog.logfile' => 'php://stderr',
));

// Register view rendering
$app->register(new Silex\Provider\TwigServiceProvider(), array(
    'twig.path' => __DIR__.'/views',
));

// Our web handlers

$app->get('/', function() use($app) {
  $app['monolog']->addDebug('logging output.');
  return $app['twig']->render('index.twig');
});

$app->get('/login', function() use($app) {
  $app['monolog']->addDebug('logging output.');
  return ('Estamos construyendo la página de login');
});

$app->get('/consulta', function() use($app) {
 	$conexion = pg_connect("host=ec2-107-20-153-39.compute-1.amazonaws.com port=5432 dbname=d8r3vjhhkehuv4 user=ybklwjsgmubonm password=9fd44fba109201c501e9ee0bac95f99c73b66dca9f13f0a45c0949f5b0ed9b8a");

 	$consulta = pg_query($conexion, "SELECT * FROM menu");

 	$detalle = pg_fetch_result($consulta,2,2);

 	return $detalle;

});

$app->get('/guardarDato/{node}/{estado}', function($node, $estado) use($app) {
 	$conexion = pg_connect("host=ec2-107-20-153-39.compute-1.amazonaws.com port=5432 dbname=d8r3vjhhkehuv4 user=ybklwjsgmubonm password=9fd44fba109201c501e9ee0bac95f99c73b66dca9f13f0a45c0949f5b0ed9b8a");

 	$datetime = date("Y-m-d H:i:s");

 	$datos = array(	"fecha" => $datetime, 
 					"estado" => $estado, 
 					"node" => $node);

 	$insertar = pg_insert($conexion,"plazas",$datos);


 	return $insertar;
});

$app->get('/consultarPlaza/{plaza}', function($plaza) use($app) {
 	$conexion = pg_connect("host=ec2-107-20-153-39.compute-1.amazonaws.com port=5432 dbname=d8r3vjhhkehuv4 user=ybklwjsgmubonm password=9fd44fba109201c501e9ee0bac95f99c73b66dca9f13f0a45c0949f5b0ed9b8a");

 	$query = "SELECT * FROM plazas WHERE node=".$plaza."ORDER BY fecha DESC LIMIT 1";
 	$consulta = pg_query($query);
 	$datos = pg_fetch_row($consulta);
 	return $datos[2];
});

$app->get('/liberarEspacio/{tabla}', function($tabla) use($app) {
 	$conexion = pg_connect("host=ec2-107-20-153-39.compute-1.amazonaws.com port=5432 dbname=d8r3vjhhkehuv4 user=ybklwjsgmubonm password=9fd44fba109201c501e9ee0bac95f99c73b66dca9f13f0a45c0949f5b0ed9b8a");

 	$query = "SELECT * FROM ".$tabla;
 	$consulta = pg_query($conexion, $query);
 	$filas = pg_num_rows($consulta);
 	$respuesta=pg_fetch_all($consulta)
 	$id=$respuesta[0][0];

 	if($filas>60){
 		$query2 = 'DELETE FROM '. $tabla .' WHERE id <'.$id - 20;
 		$consulta2 = pg_query($conexion, $query2);
 		return $consulta2;
 	}
 	else{
 		return "Aún no es necesario borrar datos";
 	}
});

$app->run();
