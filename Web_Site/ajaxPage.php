<?php
	// Data from client (filtered ajax)
	if (isset($_POST['data'])) {
		$data = json_decode($_POST['data'], true);
	}
	$baliseTab = NULL;
	if (isset($data['tableBalise'])) {
		$baliseTab = $data['tableBalise'];
	}
	$balisePostionTab = NULL;
	if (isset($data['tableBalisePostion'])) {
		$balisePostionTab = $data['tableBalisePostion'];
	}
	$interdictionTab = NULL;
	if (isset($data['tableInterdiction'])) {
		$interdictionTab = $data['tableInterdiction'];
	}
	// Open DB
	include_once("dbConfig.php");
	$mysqli = new mysqli(DB_HOST, DB_LOGIN, DB_PWD, DB_NAME);
	$mysqli->set_charset("utf8");

	// Query : select
	$query = "SELECT * FROM vehicule WHERE 'move' = TRUE OR 'ready' = TRUE;";
	$result = $mysqli->query($query);
	$numRows = $result->num_rows;
	$result->close();
	if(!$numRows){
		$query = "DELETE FROM map;";
		$mysqli->query($query);
		// Save pass target in db
		foreach($baliseTab as $balise){
			$id = substr($balise , 0, stripos($balise, ":"));
			$pos = stripos($balise, ":") + 1;
			$posX = substr($balise , $pos, stripos($balise, "i") - $pos);
			$posY = substr($balise , stripos($balise, "id£")+4);
			$id = $mysqli->real_escape_string($id);
			$posX = $mysqli->real_escape_string($posX);
			$posY = $mysqli->real_escape_string($posY);
			$query = "INSERT INTO map VALUES ('balise', " . $id . ", ". $posX . ", " . $posY . ");";
			$mysqli->query($query);
		}
		// Save interdiction target in db
		foreach($interdictionTab as $interdiction){
			$posX = substr($interdiction , 0, stripos($interdiction, "id£"));
			$posY = substr($interdiction , stripos($interdiction, "id£")+4);
			$posX = $mysqli->real_escape_string($posX);
			$posY = $mysqli->real_escape_string($posY);
			$query = "INSERT INTO map VALUES ('interdiction', 0, ". $posX . ", " . $posY . ");";
			$mysqli->query($query);
		}
		// Save position target in db
		foreach($balisePostionTab as $balisePosition){
			$id = "balisePosition" . substr($balisePosition , 0, stripos($balisePosition, ":"));
			$pos = stripos($balisePosition, ":") + 1;
			$posX = substr($balisePosition , $pos, stripos($balisePosition, "id") - $pos);
			$posY = substr($balisePosition , stripos($balisePosition, "id£")+4);
			$id = $mysqli->real_escape_string($id);
			$posX = $mysqli->real_escape_string($posX);
			$posY = $mysqli->real_escape_string($posY);
			$query = "INSERT INTO map VALUES ('" . $id  . "', 0, ". $posX . ", " . $posY . ");";
			$mysqli->query($query);
		}
		// balisePosition
		$query = "UPDATE vehicule SET ready = 1;";
		$success = $mysqli->query($query);
	}

	// Close DB
	$mysqli->close();
?>