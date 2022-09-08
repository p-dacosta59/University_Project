<?php
	$json = array();
	session_start();

	// Data from client (filtered ajax)
	if (isset($_POST['data']))
		$data = json_decode($_POST['data'], true);

	// Open DB
	include_once("dbConfig.php");
	$mysqli = new mysqli(DB_HOST, DB_LOGIN, DB_PWD, DB_NAME);
	$mysqli->set_charset("utf8");
	if(isset($data['turnOff'])) {
		// Query : update
		$query = "UPDATE vehicule SET ready = FALSE, powerOn = FALSE;";
		$success = $mysqli->query($query);
		$jsonPower = array("action"=>"reloadPage");
		$json[] = $jsonPower;
		$_SESSION['ifCarPowered'] = FALSE;
	}

	// Query : select 
	$query = "SELECT * FROM vehicule WHERE powerOn = TRUE";
	$result = $mysqli->query($query);
	$numRows = $result->num_rows;
	// If car turned on
	if(!$_SESSION['ifCarPowered'] && $numRows){
		$jsonPower = array("action"=>"reloadPage");
		$json[] = $jsonPower;
		$_SESSION['ifCarPowered'] = TRUE;
	}
	$row = $result->fetch_assoc();
	if($row['ready']){
		$posXVheicule = $row['posX'];
		$posYVheicule = $row['posY'];
		if ($posXVheicule != $_SESSION['posXCar'] || $posYVheicule != $_SESSION['posYCar']){
			$newPos = "#" . $posXVheicule . ":" . $posYVheicule;
			$jsonPosCar = array("newPosCar"=>$newPos);
			$_SESSION['posXCar'] = $posXVheicule;
			$_SESSION['posYCar'] = $posYVheicule;
			$json[] = $jsonPosCar;
		}
		// If new Image avaible display it
		$dataToSend = "<img src='./img" . $_SESSION['image'] . ".png'/>";
		$jsonImage = array("target"=>".image", "html" => $dataToSend);
		if($_SESSION['image'])
			$_SESSION['image'] = 0;
		else
			$_SESSION['image'] = 1;
		$json[] = $jsonImage;

		// Query : select  
		$query = "SELECT * FROM map WHERE type = 'final'";
		$result2 = $mysqli->query($query);
		$numRows = $result2->num_rows;
		if($numRows){
			$row2 = $result2->fetch_assoc();
			if($_SESSION['posFinalX'] != $row2['positionX'] || $_SESSION['posFinalY'] != $row2['positionY']){
				$jsonPosFinal = array("action"=>'reloadPage');
				$_SESSION['posFinalX'] = $row2['positionX']; 
				$_SESSION['posFinalY'] = $row2['positionY'];
				$json[] = $jsonPosFinal;
			}
			$result2->close();
		}
	}

	// Close DB
	$result->close();
	$mysqli->close();
	// Data to client (merge and send)
	echo json_encode($json);
?>