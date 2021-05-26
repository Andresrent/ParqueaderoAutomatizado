<?php

use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;

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


$app->get('/guardarDato/{node}/{estado}', function($node, $estado) use($app) {
 	$conexion = pg_connect("host=ec2-107-20-153-39.compute-1.amazonaws.com port=5432 dbname=d8r3vjhhkehuv4 user=ybklwjsgmubonm password=9fd44fba109201c501e9ee0bac95f99c73b66dca9f13f0a45c0949f5b0ed9b8a");

 	$datetime = date("Y-m-d H:i:s");

 	$datos = array(	"fecha" => $datetime, 
 					"estado" => $estado, 
 					"node" => $node);

 	$insertar = pg_insert($conexion,"plazas",$datos);


 	return $insertar;
});

$app->post('/guardarDato', function (Request $request) use ($app) {
  $estado = $request->get('estado');
  $node = $request->get('node');

  $conexion = pg_connect("host=ec2-107-20-153-39.compute-1.amazonaws.com port=5432 dbname=d8r3vjhhkehuv4 user=ybklwjsgmubonm password=9fd44fba109201c501e9ee0bac95f99c73b66dca9f13f0a45c0949f5b0ed9b8a");

 	$datetime = date("Y-m-d H:i:s");

 	$datos = array(	"fecha" => $datetime, 
 					"estado" => $estado, 
 					"node" => $node);

 	$insertar = pg_insert($conexion,"plazas",$datos);


 	return $insertar;
});

$app->post('/guardarTemp', function (Request $request) use ($app) {
  $temperatura = $request->get('temperatura');

  $conexion = pg_connect("host=ec2-107-20-153-39.compute-1.amazonaws.com port=5432 dbname=d8r3vjhhkehuv4 user=ybklwjsgmubonm password=9fd44fba109201c501e9ee0bac95f99c73b66dca9f13f0a45c0949f5b0ed9b8a");

 	$datetime = date("Y-m-d H:i:s");

 	$datos = array(	"fecha" => $datetime, 
 					"temperatura" => $temperatura);

 	$insertar = pg_insert($conexion,"temp_data",$datos);


 	return $insertar;
});


$app->get('/consultarPlaza/{plaza}', function($plaza) use($app) {
 	$conexion = pg_connect("host=ec2-107-20-153-39.compute-1.amazonaws.com port=5432 dbname=d8r3vjhhkehuv4 user=ybklwjsgmubonm password=9fd44fba109201c501e9ee0bac95f99c73b66dca9f13f0a45c0949f5b0ed9b8a");

 	

  if($plaza=="todas"){
      $estados = array();

      try {
          for($i=1;$i<=2;$i++){
            $query = "SELECT * FROM plazas WHERE node=".$i." ORDER BY fecha DESC LIMIT 1";
            $consulta = pg_query($conexion,$query);
            $datos = pg_fetch_row($consulta);
            $index=$i+1;
            $estado_n=array("plaza"=>$i,"estado"=>$datos[2]);
            array_push($estados, $estado_n);
          }

          $jsonResult = json_encode($estados, JSON_PRETTY_PRINT | JSON_FORCE_OBJECT);

          $response = new Response();
          $response->setContent($jsonResult);
          $response->setCharset('UTF-8');
          $response->headers->set('Content-Type', 'application/json');

          return $response;
      }
      catch (Exception $e) {
          return $e->getMessage();
      }

      
   }
   elseif($plaza=="disponibles"){
      $totalDisponible=0;
      for($i=1;$i<=2;$i++){
          $query = "SELECT * FROM plazas WHERE node=".$i."ORDER BY fecha DESC LIMIT 1";
          $consulta = pg_query($conexion,$query);
          $datos = pg_fetch_row($consulta);
          if($datos[2]==0){
            $totalDisponible=$totalDisponible+1;
          }
      }
      return $totalDisponible;
   }
   else{
    try{
        $query = "SELECT * FROM plazas WHERE node=".$plaza."ORDER BY fecha DESC LIMIT 1";
        $consulta = pg_query($conexion,$query);
        $datos = pg_fetch_row($consulta);
        return $datos[2];
      }
      catch (Exception $e) {
            return "No recibí un valor para consultar. ERROR: " .$e->getMessage();
      }
    }
    
});



$app->run();
