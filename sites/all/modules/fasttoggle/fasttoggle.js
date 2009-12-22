// $Id: fasttoggle.js,v 1.2.2.3 2007/07/05 14:08:45 timcn Exp $

Drupal.fasttoggle = {
  'comment': function(data) {
    if (data.option == 'status') {
      $(this).parents('.comment')[data.status ? 'addClass' : 'removeClass']('comment-unpublished');
    }
  },

  'node': function(data) {
    var node = $(this).parents('.node');

    if (data.option == 'sticky') {
      node[data.status ? 'addClass' : 'removeClass']('sticky');
    }
    else if (data.option == 'status') {
      node[data.status ? 'removeClass' : 'addClass']('node-unpublished');
    }
  }
};

if (Drupal.jsEnabled) {
  $(function() {
    $('a.fasttoggle').unclick().click(function() {
      // Add the throbber
      var link = $(this).addClass('throbbing');

      // Perform a request to the server
      jQuery.post(this.href, { confirm: true, javascript: true }, function(data) {
        data = Drupal.parseJson(data);
        if (data) {
          // Remove the throbber
          link.html(data.text).removeClass('throbbing');

          // Call the callback function for altering the display of other elements
          if (data.callback && Drupal.fasttoggle[data.callback]) {
            Drupal.fasttoggle[data.callback].call(link[0], data);
          }
        }
      });

      // Do not execute the regular functionality when the user clicks the link
      return false;
    });
  });
}