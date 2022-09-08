$(document).ready(function(){
//==============================================================================

	/*==============================================================================
		jQuery events
	==============================================================================*/

	var cursor = "balise";
	var id = 0;
	
	// Identify type of cursor and change for user
	jQuery("body").on("click", ".balise", function() {
		jQuery(".wrapper").css("cursor", "url", "'../medias/balise.png'", "auto");
		cursor = "balise";
	});	

	jQuery("body").on("click", ".interdictions", function() {
		jQuery(".wrapper").css("cursor", "url", "'../medias/interdiction.png'", "auto");
		cursor = "interdiction";
	});

	jQuery("body").on("click", ".balisePositions", function() {
		jQuery(".wrapper").css("cursor", "url", "'../medias/tagPosition.png'", "auto");
		cursor = "balisePosition";
	});

	jQuery("body").on("click", ".delete", function() {
		jQuery(".wrapper").css("cursor", "url", "'../medias/delete.png'", "auto");
		cursor = "delete";
	});

	jQuery("body").on("click", "section input", function(key) {
		if(document.getElementsByClassName("picture").length > 1)
			cursor = "";
	});

	// Add tag used in map with a picture and a class to differenciate
	jQuery("body").on("click", "section table td", function() {
		if(cursor != "" && document.getElementsByClassName('delete').length && !jQuery(this).hasClass("car")){
			if(cursor == "interdiction"){
				if(jQuery(this).hasClass("picture"))
					removeTag(this);
				jQuery(this).html("<img src='./medias/interdiction.png'/>");
				jQuery(this).addClass('picture interdiction');
			}else if(cursor == "balise"){
				if(jQuery(this).hasClass("picture"))
					removeTag(this);
				jQuery(this).html("<img src='./medias/balise.png'/>");
				jQuery(this).addClass('picture');
				jQuery(this).prepend("<p style=\"display: none;\">" + id.toString() + "</p>");
				id++;
			}else if(cursor == "balisePosition"){
				if(jQuery(this).hasClass("picture"))
					removeTag(this);
				jQuery(this).html("<img src='./medias/tagPosition.png'/>");
				jQuery(this).addClass('picture waitName balisePosition');
				jQuery("body").prepend("<div class='popUp'><div class='picturePop'><input type='text' maxlength = '1'><p>Envoyer</p></div></div>");
				jQuery(".picturePop input").focus();
				jQuery(".wrapper").css("opacity", "0.2");
				jQuery(".wrapper").css("z-index", "-1");
			}else if(cursor == "delete")
				removeTag(this);
		}
	});

	// Display a popUp to send the target name with is position to triangulize the car position
	jQuery("body").on("click", ".popUp p", function(){
		if(jQuery(".popUp input").val() != ""){
			var table = document.getElementsByClassName("balisePosition");
			for(var i=1; i < table.length; i++){
				if(jQuery("#" + table[i].id).children("p").html() == jQuery(".popUp input").val())
					removeTag("#" + table[i].id);
			}
			jQuery(".waitName").prepend("<p style=\"display: none;\">" + jQuery(".popUp input").val() + "</p>");
			jQuery(".waitName").removeClass("waitName");
			jQuery(".popUp").remove();
			jQuery(".wrapper").css("opacity", "0.99");
			jQuery(".wrapper").css("z-index", "1");
		}
	});
	
	// Remove tag in a target in map
	jQuery("body").on("contextmenu", "section table td", function() {
		if(cursor != "" && document.getElementsByClassName('delete').length && !jQuery(this).hasClass("car"))
			removeTag(this);
	});

	// Add a tag
	jQuery("body").on("click", ".addTagPosition", function() {
		jQuery(this).append();
	});

	// Add target behind the mouse when moove
	jQuery("body").on("mousemove", "section table td", function() {
		var posX = this.id[0];
		var posY;
		var otherInf = "";
		if(this.id[1] != "i"){
			posX += this.id[1];
			posY = this.id[5];
			if(this.id[6])
				posY += this.id[6];
		}else{
			posY = this.id[4];
			if(this.id[5])
				posY += this.id[5];
		}
		if(jQuery(this).hasClass("picture") && !jQuery(this).hasClass("interdiction"))
			otherInf = "balise " + jQuery(this).children("p").html();
		jQuery(".mousePosition").html(posX + ":" + posY + "     " + otherInf);
	});

	// Remove a tag when user delete the target
	function removeTag(position) {
		if(jQuery(position).hasClass("picture") && !jQuery(this).hasClass("car")){
			var s = jQuery(position).children("p").html();
			jQuery(position).html("");
			jQuery(position).removeClass('picture');
			if(!jQuery(position).hasClass("interdiction") && !jQuery(position).hasClass("balisePosition")){
				for(s++; s < id; s++){
					var table = document.getElementsByClassName("picture");
					for(var i=1; i < table.length; i++){
						if(jQuery("#" + table[i].id).children("p").html() == s)
							jQuery("#" + table[i].id).children("p").html((s-1).toString());
					}
				}
				id--;
			}
		}
	}
});