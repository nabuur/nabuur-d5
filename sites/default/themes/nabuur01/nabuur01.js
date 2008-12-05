  /*
   * $(document).ready();
   */
  
  /**
   * Not used right now. Needed for column with background.
   * Here to have it when needed
   */
  function align_columns() {
    /* div prefix needed for IE5.5 */
    var c_height = $('div#content').height();
    var r_height = $('div#right').height();
    if (c_height < r_height) {
      $('div#content').height(r_height+'px');
    }
    else {
      $('div#right').height(c_height+'px');
    }
  }

  /**
   * Show or hide the login popup
   */
  function show_login(show) {
    if (show == true) {
      $('#user_login').show();
    }
    else {
      $('#user_login').hide();
    }
  }
