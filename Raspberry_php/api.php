<?php
/*
 * Umbrella Rental System
 * Copyright 2020 a5768549
 *
 * Raspberry PHP
 * This code responsible for repeat between server(database) and client
 * 
 * GitHub Project：https://github.com/a5768549/Umbrella-Rental-System
 */

include('connect.php');
$data   =  json_decode(file_get_contents('php://input'),true);

$type   = $data["type"];
$action = $data["action"];
$device = $data["device"];
$data   = $data["data"];

$postdata = http_build_query(
    array(
        'type'   => $type,
		'action' => $action,
        'device' => $device,
		'data'   => $data
    )
);

$opts = array('http' =>
    array(
        'method'  => 'POST',
        'header'  => 'Content-Type: application/x-www-form-urlencoded',
        'content' => $postdata
    )
);

$context = stream_context_create($opts);
$result  = file_get_contents($DB_path.'/api.php', false, $context);

echo $result;
?>
