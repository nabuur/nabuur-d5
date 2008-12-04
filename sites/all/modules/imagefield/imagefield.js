// $Id: imagefield.js,v 1.1.2.1 2008/04/21 06:59:13 dopry Exp $

/**
 *  Auto Attach standard client side file input validation
 */
Drupal.imagefieldValidateAutoAttach = function() {
  $("input[@type='file'].imagefield").change( function() {
    $('.imagefield-js-error').remove();
    /**
     *  add client side validation for the input[@file] accept attribute
     */
 
    if(this.accept.length>1){
      v = new RegExp('\\.('+(this.accept?this.accept:'')+')$','gi');
      if (!v.test(this.value)) {
        var error = 'The file ' + this.value + " is not supported.\n";
        error += "Only the following file types are supported: \n" + this.accept.replace(/\|/g, ' ');
        alert(error);
        // what do I prepend this to? 
        //$(this).prepend($('<div class="imagefield-js-error>"' + error + '</div>'));
        this.value = ''; 
        return false;
      }   
    }   
    /**
     * Add filesize validation where possible
     */

  }); 
}

// Global killswitch
if (Drupal.jsEnabled) {
  $(document).ready(Drupal.imagefieldValidateAutoAttach);
}

