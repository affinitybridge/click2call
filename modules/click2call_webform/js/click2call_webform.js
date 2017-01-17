(function ($) {
  Drupal.behaviors.click2callWebform = {
    attach: function (context, settings) {
      var click2callKey = settings.click2call.c2cKey;
      $("#click2call-"+click2callKey+"-callcid").change(function () {
        var callCid = $("#click2call-"+click2callKey+"-callcid").val();
        $(".webform-component-click2call input[name='submitted[click2call]']").val(callCid);
      });
      $("#click2call-button-"+click2callKey).click();
      $("input.webform-submit").attr('disabled', true);
      $("input.webform-submit").addClass('form-button-disabled');
      $("#click2call-"+click2callKey+"-status").change(function () {
        $("input.webform-submit").removeAttr('disabled');
        $("input.webform-submit").removeClass('form-button-disabled');
        $("#click2call-button-"+click2callKey).hide();
      });
    }
  };

}(jQuery));
