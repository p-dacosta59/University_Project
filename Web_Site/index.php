<!DOCTYPE html>

<html>
	<!-- Head -->
	<head>
		<!-- CSS files -->
		<link rel='stylesheet' type='text/css' href='./css/web.css' media='screen' />
		<link rel='stylesheet' type='text/css' href='./css/01_mobile.css' media='screen' />
		<link rel='stylesheet' type='text/css' href='./css/02_fonts.css' media='screen' />

		<!-- JS files -->
		<script type='text/javascript' src='./js/jquery-3.5.1.min.js'></script>
		<script type='text/javascript' src='./js/ajax.js'></script>
		<script type='text/javascript' src='./js/web.js'></script>

		<!-- UTF8 encoding -->
		<meta charset='UTF-8'>

		<!-- Prevent from zooming -->
		<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1, user-scalable=0,  shrink-to-fit=no">

		<!-- Icon -->
		<link rel='icon' type='image/png' href='./images/favicon.png' />

		<!-- Title -->
		<title>Controleur Voiture</title>
	</head>

	<!-- Body -->
	<body>
		<!-- Wrapper -->
		<div class='wrapper'>
			<header>
				<h1>Controleur Voiture</h1>
			</header>
			<section>
				<h2>Bienvenue</h2>
				<h3>Map</h3>
				<table id="myTable">
					<tbody>
						<?php 
							// Open DB
							include_once("dbConfig.php");
							$mysqli = new mysqli(DB_HOST, DB_LOGIN, DB_PWD, DB_NAME);
							$mysqli->set_charset("utf8");
							// Query : select
							$query = "SELECT * FROM vehicule WHERE powerOn = True;";
							$result1 = $mysqli->query($query);
							$row = $result1->fetch_assoc();
							$posXVheicule = $row['posX'];
							$posYVheicule = $row['posY'];
							session_start();
							$_SESSION['posXCar'] = $posXVheicule;
							$_SESSION['posXYCar'] = $posYVheicule;
							$ifCarPowered = $result1->num_rows;
							$result1->close();
							// If car turned on display map with position and target 
							if($ifCarPowered){
								$_SESSION['ifCarPowered'] = TRUE;
								$query = "SELECT * FROM vehicule WHERE ready = FALSE;";
								$result = $mysqli->query($query);
								$numRows = $result->num_rows;
								$result->close();
								// Select all target to display
								if(!$numRows){
									$query = "SELECT * FROM map ORDER BY positionY ASC, positionX ASC";
									$result2 = $mysqli->query($query);
									$row = $result2->fetch_assoc();
									for($i = 0; $i < 49; $i++){
										echo "<tr>";
										for($j = 0; $j < 49; $j++){
											// Display in tab
											if($posXVheicule == $j && $posYVheicule == $i){
												echo "<td class = 'car picture' id='" . $j . "id£" . $i . "'><img src='./medias/vehicule.png'/></td>";
											}else if($j == $row['positionX'] && $i == $row['positionY']){
												if($row['type'] == 'interdiction')
													echo "<td class = 'picture' id='" . $j . "id£" . $i . "'><img src='./medias/interdiction.png'/></td>";
												else if($row['type'] == 'final')
													echo "<td class = 'picture final' id='" . $j . "id£" . $i . "'><img src='./medias/balise.png'/></td>";
												else if($row['type'] == 'path')
													echo "<td class = 'path' id='" . $j . "id£" . $i . "'></td>";
												else if(substr($row['type'], 0, 14) ==  "balisePosition")
													echo "<td class = 'picture' id='" . $j . "id£" . $i . "'><p style=\"display: none;\">" . $row['type'][14] . "</p><img src='./medias/tagPosition.png'/></td>";
												else
													echo "<td class = 'picture' id='" . $j . "id£" . $i . "'><p style=\"display: none;\">" . $row['id'] . "</p><img src='./medias/balise.png'/></td>";
												$row = $result2->fetch_assoc();
											}else
												echo "<td id='" . $j . "id£" . $i . "'></td>";
										}
										echo "</tr>";
									}
									$result2->close();
								}else{
									// Display a normal map
									for($i = 0; $i < 49; $i++){
										echo "<tr>";
										for($j = 0; $j < 49; $j++){
											if($j == $posXVheicule && $i == $posYVheicule)
												echo "<td class = 'car picture' id='" . $i . "id£" . $j . "'><img src='./medias/vehicule.png'/></td>";
											else
												echo "<td id='" . $j . "id£" . $i . "'></td>";
										}
										echo "</tr>";
									}
								}
							}
							else{
								$_SESSION['ifCarPowered'] = FALSE;
								echo "<h2>La voiture est éteinte</h2>";
							}
							// Close DB
							$mysqli->close();
						?>
					</tbody>
				</table>
				<?php if($numRows) echo "<input type='submit' value='Envoyer le parcour'>";?>
			</section>
			<aside>
				<div>
					<h3>Légende</h3>
					<article>
						<img src='./medias/vehicule.png'/> Emplacement de votre vehicule
						<!-- If needed to add target if car turned off -->
						<p <?php if($numRows) echo "class='balise'";?>><img src='./medias/balise.png'/> Point de passage</p>
						<p <?php if($numRows) echo "class='interdictions'";?>><img src='./medias/interdiction.png'/> Point de non passage</p>
						<p <?php if($numRows) echo "class='balisePositions'";?>><img src='./medias/tagPosition.png'/> Balise de localisation</p>
						<p <?php if($numRows) echo "class='delete'";?>><img src='./medias/delete.png'/> Supression d'un point</p>
					</article>
					<h3>Image en direct</h3>
					<article class='image'>
						<img src='./img0.png'/>
					</article>
				</div>
			</aside>
			<?php if($ifCarPowered)
				echo "<div class='mousePosition'>
					</div>";
			?>
			<footer>
				<!-- You can turn off car when you want -->
				<?php if($ifCarPowered) echo "<p>Vous pouvez <a>éteindre votre véhicule</a> si vous le souhaitez</p>";?>
			</footer>
		</div>
	</body>
</html>
