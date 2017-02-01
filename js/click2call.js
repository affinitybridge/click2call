(function ($) {
  Drupal.behaviors.click2call = {
    attach: function (context, settings) {
      var click2callKey = settings.click2call.c2cKey;
      var formBuildId = settings.click2call.formBuildId;

      $("#click2call-hangup-button-"+click2callKey).attr('disabled', 'disabled');
      $("#click2call-hangup-button-"+click2callKey).hide();
      $(".click2call-"+click2callKey+"-phone").hide();

      // Display Phone Link
      $('.click2call-link').click(function () {
        click2callDisplay(click2callKey);
      });
      // Call Button
      $('.click2call-button').click(function () {
        click2callCall(click2callKey, formBuildId);
      });
      // Hangup Button
      $('.click2call-hangup-button').click(function () {
        click2callHangup(click2callKey);
      });
    }
  };

  function click2callAttachEvents() {
    //Automatically select radio when typing in textfield
    // $(".click2call-type").focus(function() {
    // 	$(this).prev().attr("checked","checked");
    // });

    //Automatically select radio when focusing on select list
    // $(".click2call-voipnumber").focus(function() {
    // 	$(this).prev().attr("checked","checked");
    // });
  }

  function click2callDisplay(click2callKey) {
    if($("#click2call-group-"+click2callKey).hasClass('on')){
      $("#click2call-group-"+click2callKey).hide();
    }
    else{
      $("#click2call-group-"+click2callKey).show();
      $("#click2call-button-"+click2callKey).show();
      $("#click2call-button-"+click2callKey).removeAttr("disabled");
    }
    $("#click2call-group-"+click2callKey).toggleClass('on');
  }

  function click2callCall(click2callKey, formBuildId) {
  	//Disable the button
  	$("#click2call-button-"+click2callKey).attr('disabled', 'disabled');
    $("#click2call-button-"+click2callKey).hide();
      //Remove error message
  	$("#click2call-"+click2callKey+"-status").html("");
  	//Upload button
  	var phone=$("#click2call-group-"+click2callKey+" input:radio:checked").val();
    if (phone == "voipnumber") {
  		//Get the phone number from select list
  		phone=$("#click2call-"+click2callKey+"-select :selected").val();
  	}
    else if (phone == "type") {
  		phone=$("#click2call-"+click2callKey+"-type-phone").val();
  	}

  	//Hide the phone number selection
  	$("#click2call-"+click2callKey+"-hidden").html($(".click2call-"+click2callKey+"-phone").html());
      $(".click2call-"+click2callKey+"-phone").html(Drupal.t("Calling..."));
      $(".click2call-"+click2callKey+"-phone").show();
  		//Enable hangup button
  		$("#click2call-hangup-button-"+click2callKey).removeAttr("disabled");
      $("#click2call-hangup-button-"+click2callKey).show();

  		$.ajax({
  			type: "GET",
  			url: Drupal.settings.basePath + "click2call/call/" + formBuildId,
//  			data: queryString,
  			dataType: 'json',
  			success: function(data){
  						click2callResponse(data, click2callKey);
  					}
  		});
  }

  function click2callHangup(click2callKey) {
  	var call_cid = $("#click2call-"+click2callKey+"-callcid").val();

  	$.ajax({
      type: "GET",
      url: Drupal.settings.basePath +"?q=click2call/hangup/" + call_cid + "/click2callKey",
      dataType: 'json',
      success: function(data){
        if(data.status){
          $(".click2call-"+click2callKey+"-phone").html(Drupal.t("Terminating call..."));
          $("#click2call-button-"+click2callKey).removeAttr("disabled");
          $("#click2call-button-"+click2callKey).show();
          $("#click2call-hangup-button-"+click2callKey).attr("disabled", "disabled");
          $("#click2call-hangup-button-"+click2callKey).hide();
          $(".click2call-"+click2callKey+"-phone").hide();
          $("#click2call-"+click2callKey+"-status").trigger("change");
        }
      }
  	});
  }

  function click2callResponse(data, click2callKey){
    var callCid = data.callCid;
  	$("#click2call-"+click2callKey+"-callcid").val(callCid).trigger('change');
  	// Check for call status after 3 sec.
  	window.setTimeout(function() {
  		click2callCheck(callCid, click2callKey);
  	}, 3000);
  }

  function click2callCheck(call_cid, click2callKey) {
    $.ajax({
  		type: "GET",
  		url: Drupal.settings.basePath +"?q=click2call/status/" + call_cid,
  		dataType: 'json',
  		success: function(data){
        if (data.status == "success") {
          //Call is sucess
          $(".click2call-"+click2callKey+"-phone").html($("#click2call-"+click2callKey+"-hidden").html());
          $("#click2call-"+click2callKey+"-hidden").html("");
          click2callAttachEvents();
          $("#click2call-"+click2callKey+"-status").html(data.message);
          $("#click2call-button-"+click2callKey).removeAttr("disabled");
          $("#click2call-hangup-button-"+click2callKey).attr('disabled', 'disabled');
          $("#click2call-hangup-button-"+click2callKey).hide();
          $("#click2call-"+click2callKey+"-status").trigger("change");
        }
        else if (data.status == "failed") {
drupal_set_message('The Call failed!', 'warning');
          //Call is terminated (busy, not answered, error, unavailable, ...)
          $(".click2call-"+click2callKey+"-phone").html($("#click2call-"+click2callKey+"-hidden").html());
          $("#click2call-"+click2callKey+"-hidden").html("");
          click2callAttachEvents();
          $("#click2call-"+click2callKey+"-status").html(data.message);
          $("#click2call-button-"+click2callKey).removeAttr("disabled");
          $("#click2call-hangup-button-"+click2callKey).attr('disabled', 'disabled');
          $("#click2call-"+click2callKey+"-status").trigger("change");
        }
        else {
          //Call is not finished yet, check again after 2 sec.
          window.setTimeout(function() {
            click2callCheck(call_cid, click2callKey);
          }, 2000);
        }
      }
    });
  }

}(jQuery));
