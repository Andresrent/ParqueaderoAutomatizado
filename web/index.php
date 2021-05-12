<?php

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

 	return $consulta;

});

$app->run();
