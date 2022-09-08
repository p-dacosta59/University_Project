$(document).ready(function(){
//==============================================================================

/*==============================================================================
	jQuery events
==============================================================================*/

	// Ajax polling every 1s to update the map
	window.setInterval(sendAjax, 1000, "ajaxCar.php");

	// When send the map to the car recover all tag interdiction and Position
	jQuery("body").on("click", "section input", function(key) {
		var table;
		var tableBalise = [];
		var tableInterdiction = []; 
		var tableBalisePostion = [];
		var j = 0;
		var loopEmpty = true;
		table = document.getElementsByClassName("picture");
		while(loopEmpty){
			loopEmpty = false;
			for(var i=1; i < table.length; i++)
				if(jQuery("#" + table[i].id).children("p").html() == j.toString()){
					tableBalise[tableBalise.length] = j.toString() + ":" + table[i].id;
					j++;
					loopEmpty = true;
				}
		}
		// Differentiate tag type
		table = document.getElementsByClassName("balisePosition");
		for(var i=0; i < table.length; i++){
			tableBalisePostion[tableBalisePostion.length] = jQuery("#" + table[i].id).children("p").html() + ":" + table[i].id;
		}
		table = document.getElementsByClassName("interdiction");
		for(var i=0; i < table.length; i++){
			tableInterdiction[tableInterdiction.length] = table[i].id;
		}
		// Send table to the PHP to save in the db
		if(tableBalise.length > 0){
			jQuery(this).hide();
			jQuery("aside").children('p').removeClass();
			sendAjax("ajaxPage.php", {tableBalise, tableInterdiction, tableBalisePostion});
		}
	});

	jQuery("body").on("click", "footer a", function(key) {
		sendAjax("ajaxCar.php", {turnOff: true});
	});

/*==============================================================================
	AJAX functions
==============================================================================*/

	// General function sending JSON data to server
	function sendAjax(serverUrl, jsonData) {
		serializedData = JSON.stringify(jsonData);
		jQuery.ajax({type: 'POST', url: serverUrl, dataType: 'json', data: "data=" + serializedData,
			success: function(data) {
				displayAjax(data);
			}
		});
	}

	// --- General function displaying JSON data from server
	// Syntax from server : [{target:".htmlClass", html:"html"}, {target:"htmlElement", html:"html", insert:"append"}, {action:"reloadPage"}]
	function displayAjax(data) {
		for (var val of data) {
			// Insert data into HTML (insert or replace)
			if (defined(val.target) && defined(val.html)) {
				if (val.insert == "prepend"){
					jQuery(val.target).prepend(val.html);
				} else if (val.insert == "append") {
					jQuery(val.target).append(val.html);
				}else if(val.insert == "after"){
					jQuery(val.target).after(val.html);
				}else if(val.insert == "before"){
					jQuery(val.target).before(val.html);
				} else {
					jQuery(val.target).html(val.html);
				}
			}
			// Change the position of car in map
			else if(defined(val.newPosCar)){
				var pictureCar = jQuery(".car").html();
				jQuery(".car").html("");
				jQuery('.car').toggleClass();
				jQuery(val.newPosCar).toggleClass("car picture");
				jQuery(val.newPosCar).html(pictureCar);
			}
			// Reload page
			else if (val.action == "reloadPage") {
				location.reload();
			}
		}
	}

	// --- Test whether a variable is defined or not
	function defined(myVar) {
		if (typeof myVar != 'undefined') return true;
		return false;
	}

//==============================================================================
});
