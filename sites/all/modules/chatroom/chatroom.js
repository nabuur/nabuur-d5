// $Id: chatroom.js,v 1.48.2.33 2007/10/10 15:07:05 darrenoh Exp $

Drupal.chatroom = Drupal.chatroom || {};
Drupal.chatroom.chat = Drupal.chatroom.chat || {};

/**
 * function to add chatroom events handlers to the onscreen widgets
 */
Drupal.chatroom.chat.addEvents = function() {
  Drupal.chatroom.chat.setUserColours();
  $('#chatroom-msg-submit').click(
    function() {
      Drupal.chatroom.chat.sendMessage();
      return false;
    }
  );
  $('#chatroom-msg-input').keyup(
    function(e) {
      Drupal.chatroom.chat.inputOnkeyup(this, e);
      return true;
    }
  );
  $('#chatroom-msg-away').click(
    function() {
      Drupal.chatroom.chat.setAway(this);
    }
  );
  for (var i = 0; i < Drupal.settings.chatroom.chatUsers.length; i++) {
    if (Drupal.settings.chatroom.chatUsers[i].self) {
      $('#chatroom-msg-away').attr('checked', Drupal.settings.chatroom.chatUsers[i].away);
    }
  }
  // Handling focus
  Drupal.settings.chatroom.pageTitle = document.title;
  Drupal.settings.chatroom.hasfocus = true;
  $(self).focus(
    function() {
      clearInterval(Drupal.settings.chatroom.warnInterval);
      Drupal.settings.chatroom.hasfocus = true;
      document.title = Drupal.settings.chatroom.pageTitle;
    }
  );
  $(self).blur(
    function() {
      Drupal.settings.chatroom.hasfocus = false;
    }
  );
  Drupal.chatroom.chat.getUpdates();
  Drupal.chatroom.chat.updates = setInterval("Drupal.chatroom.chat.getUpdates()", Drupal.settings.chatroom.chatUpdateInterval);
  return;
}

/**
 * handles response from msg HTTP requests
 */
Drupal.chatroom.chat.msgCallback = function(responseText) {
  if (responseText) {
    var response = eval('('+ responseText +')');
    if (typeof response == 'object') {
      if (response.cacheTimestamp != undefined) {
        Drupal.settings.chatroom.cacheTimestamp = response.cacheTimestamp;
      }
      if (response.msgs != undefined && response.msgs.length > 0) {
        Drupal.chatroom.chat.updateMsgList(response.msgs);
      }
      if (response.chatUsers != undefined) {
        Drupal.chatroom.chat.updateChatUsers(response.chatUsers);
      }
    }
  }
  return;
}

/**
 * function to handle input to the message board
 */
Drupal.chatroom.chat.inputOnkeyup = function(input, e) {
  if (!e) {
    e = window.event;
  }
  switch (e.keyCode) {
    case 13:  // return/enter
      Drupal.chatroom.chat.sendMessage();
      break;
  }
}

/**
 * sends text of message to the server
 */
Drupal.chatroom.chat.sendMessage = function() {
  var text = $('#chatroom-msg-input').val();
  if (text == '' || text.match(/^\s+$/)) {
    $('#chatroom-msg-input').val('');
    $('#chatroom-msg-input')[0].focus();
    return;
  }
  $('#chatroom-msg-input').val('');
  $('#chatroom-msg-input')[0].focus();
  if (text.search(/^\/(me|away|msg|back|kick|close|ban)/) != -1) {
    var msg = Drupal.chatroom.chat.getCommandMsg(text);
    if (!msg) {
      return;
    }
  }
  else {
    var msg = {chatroomMsg:text};
  }
  Drupal.settings.chatroom.skipUpdate = true;
  Drupal.settings.chatroom.updateCount++;
  $.post(Drupal.chatroom.chat.getUrl('write'), Drupal.chatroom.chat.prepareMsg(msg), Drupal.chatroom.chat.msgCallback);
}

/**
 * prepare a msg object
 */
Drupal.chatroom.chat.prepareMsg = function(msg) {
  if (Drupal.settings.chatroom.smileysBase) {
    msg.smileys_base = Drupal.settings.chatroom.smileysBase;
  }
  msg.drupal_base = Drupal.settings.chatroom.drupalBase;
  msg.base_url = Drupal.settings.chatroom.baseUrl;
  msg.chatroom_base = Drupal.settings.chatroom.chatroomBase;
  msg.update_count = Drupal.settings.chatroom.updateCount;
  msg.user_base = Drupal.settings.chatroom.userBase;
  msg.chat_id = Drupal.settings.chatroom.chatId;
  msg.last_msg_id = Drupal.settings.chatroom.lastMsgId;
  msg.timezone = Drupal.settings.chatroom.timezone;
  msg.timestamp = Drupal.settings.chatroom.cacheTimestamp;
  msg.chat_cache_file = Drupal.settings.chatroom.chatCacheFile;
  return msg;
}

/**
 * get a command msg object
 */
Drupal.chatroom.chat.getCommandMsg = function(text) {
  var msg = {type:text.replace(/^\//, '').split(' ')[0]};
  var args = text.replace(/^\//, '').split(' ').slice(1);
  switch (msg.type) {
    case 'me':
      // no point sending an empty message
      if (!args.length) {
        return false;
      }
      msg.chatroomMsg = args.join(' ');
      return msg;

    case 'msg':
    case 'kick':
    case 'ban':
      if (!args.length) {
        return false;
      }

      // try to find the user with this name
      var user = '';
      do {
        user += (user ? ' ' : '') + args.shift();
        msg.recipient = Drupal.chatroom.chat.findName(user).guestId;
      } while (!msg.recipient && args.length);

      // no one with this name in the chat
      if (!msg.recipient) {
        return false;
      }

      if (msg.type == 'ban') {
        msg.uid = Drupal.chatroom.chat.findGuestId(msg.recipient).uid;
        // can only ban Drupal users, not guests
        if (!msg.uid) {
          return false;
        }
        msg.admin_uid = Drupal.settings.chatroom.chatUsers[0].uid;
      }

      if (msg.type == 'msg') {
        // no point sending an empty message
        if (!args.length) {
          return false;
        }
        msg.chatroomMsg = args.join(' ');
      }
      return msg;

    case 'away':
    case 'back':
      $('#chatroom-msg-away').attr('checked', msg.type == 'away');
      msg.chatroomMsg = msg.type;
      return msg;
  }
}

/**
 * Find a user by guest ID.
 */
Drupal.chatroom.chat.findGuestId = function(guestId) {
  for (var i = 0; i < Drupal.settings.chatroom.chatUsers.length; i++) {
    if (Drupal.settings.chatroom.chatUsers[i].guestId == guestId) {
      return Drupal.settings.chatroom.chatUsers[i];
    }
  }
  return false;
}

/**
 * Find a user by user name.
 */
Drupal.chatroom.chat.findName = function(name) {
  for (var i = 0; i < Drupal.settings.chatroom.chatUsers.length; i++) {
    if (Drupal.settings.chatroom.chatUsers[i].user == name) {
      return Drupal.settings.chatroom.chatUsers[i];
    }
  }
  return false;
}

/**
 * return the appropriate url
 */
Drupal.chatroom.chat.getUrl = function(type) {
  switch (type) {
    case 'read':
    case 'write':
    case 'command':
      return Drupal.settings.chatroom.updateUrl;
    case 'user':
      return Drupal.settings.chatroom.userUrl;
  }
}

/**
 * updates message list with response from server
 * To do: Move msgs formating to php so things like using t() can be done
 */
Drupal.chatroom.chat.updateMsgList = function(msgs) {
  var msgBoard = $('#chatroom-board');
  var scroll = false;
  var newMsgUser = false;
  for (var i = 0; i < msgs.length; i++) {
    if (Drupal.chatroom.chat.updateLastMsg(msgs[i].id)) {

      var p = $('<p>');

      if (Drupal.settings.chatroom.updateCount > 1) {
        // get the user for the first message
        if (newMsgUser == false) {
          newMsgUser = msgs[i].user;
        }
        // make sure the sender of this message is not set as away
        Drupal.chatroom.chat.setAsBack(msgs[i].user);

        if (msgs[i].type == 'me') {
          // * <username> msg - italicised
          p.addClass('chatroom-me-msg');
        }
        else {
          // normal msg
          p.addClass('chatroom-msg');
          p.css('color', Drupal.chatroom.chat.getUserColour(msgs[i].user));
        }
      }
      else {
        if (msgs[i].type == 'me') {
          // * <username> msg - italicised
          p.addClass('chatroom-old-me-msg');
        }
        else {
          // first load of page - greys out old msgs
          p.addClass('chatroom-old-msg');
        }
      }

      if (msgs[i].recipient) {
        // private msg
        var span = $('<span>');
        span.addClass('header');
        span.html(msgs[i].user);

        var privSpan = $('<span>');
        var recipient = Drupal.chatroom.chat.findGuestId(msgs[i].recipient);
        Drupal.settings.chatroom.chatUsers
        if (recipient && !recipient.self) {
          privSpan.html(' (privately to '+ recipient.user +')');
        }
        else {
          privSpan.html(' (privately)');
        }
        privSpan.addClass('chatroom-private');
        span.append(privSpan);
        span.append(':');
        p.append(span);
      }
      else if (msgs[i].type == 'me') {
        // * <username> msg
        p.append('<html>* '+ msgs[i].user +' </html>');
      }
      else {
        // normal msg
        if (msgs[i].user != Drupal.settings.chatroom.lastUser) {
          var span = $('<span>');
          span.addClass('header');
          span.append(msgs[i].user + ':');
          p.append(span);
        }
      }
      p = Drupal.chatroom.chat.addSmileys(p, msgs[i].text);

      // add to board
      msgBoard.append(p);
      Drupal.settings.chatroom.lastMsgTime = msgs[i].time;

      // checking identation
      if (msgs[i].recipient || msgs[i].type == 'me') {
      	Drupal.settings.chatroom.lastUser = '';
      }
      else {
      	Drupal.settings.chatroom.lastUser = msgs[i].user;
      }

      scroll = true;

      if ($('#chatroom-msg-alert').length > 0 && $('#chatroom-msg-alert').attr('checked')) {
        Drupal.chatroom.soundManager.play('message');
        if (Drupal.settings.chatroom.hasfocus == false) {
          Drupal.settings.chatroom.newMsgInfo = msgs[i].user +': '+ Drupal.chatroom.chat.removeSmileys(msgs[i].text);
          clearInterval(Drupal.settings.chatroom.warnInterval);
          Drupal.settings.chatroom.warnInterval = setInterval("Drupal.chatroom.chat.warnNewMsgLoop()", 1500);
        }
      }
    }
  }
  if (Drupal.settings.chatroom.updateCount == 1) {
    Drupal.settings.chatroom.lastUser = '';
  }
  else {
    Drupal.chatroom.chat.setWriteTime();
  }

  if (scroll) {
    msgBoard[0].scrollTop = msgBoard[0].scrollHeight;
  }

}

/**
 * set a user as back if they send a message
 */
Drupal.chatroom.chat.setAsBack = function(user) {
  for (var i = 0; i < Drupal.settings.chatroom.chatUsers.length; i++) {
    if (Drupal.settings.chatroom.chatUsers[i].user == user) {
      // update the online list
      Drupal.settings.chatroom.chatUsers[i].away = 0;
      var guestId = Drupal.settings.chatroom.chatUsers[i].guestId;

      // update on screen display
      var guestIdObj = $('#'+ guestId);
      if (guestIdObj.length > 0) {
        guestIdObj.removeClass('chatroom-user-away');
      }

      // make sure away box is unchecked
      if (Drupal.settings.chatroom.chatUsers[i].self) {
        $('#chatroom-msg-away').attr('checked', 0);
      }
      return;
    }
  }
}

/**
 * Process smileys in text and append text to node.
 */
Drupal.chatroom.chat.addSmileys = function(node, text) {
  if (Drupal.settings.chatroom.smileysBase == undefined || text.indexOf(Drupal.settings.chatroom.smileysMarker) == -1) {
    node.append(text);
  }
  else {
    var bits = text.split(Drupal.settings.chatroom.smileysMarker);
    for (var i = 0; i < bits.length; i++) {
      var bit = $('#'+ bits[i]);
      if (bit.length > 0) {
        var smiley = bit.clone();
        smiley.removeAttr('title');
        smiley.removeAttr('id');
        bits[i] = smiley;
      }
      else {
        bits[i] = $('<html>'+ bits[i] +'</html>');
      }
      node.append(bits[i]);
    }
  }
  return node;
}

/**
 * Replace smileys markers with text smileys.
 */
Drupal.chatroom.chat.removeSmileys = function(text) {
  if (Drupal.settings.chatroom.smileysBase != undefined && text.indexOf(Drupal.settings.chatroom.smileysMarker) != -1) {
    var bits = text.split(Drupal.settings.chatroom.smileysMarker);
    for (var i = 0; i < bits.length; i++) {
      var bit = $('#'+ bits[i]);
      if (bit.length > 0) {
        bits[i] = bit.attr('alt');
      }
    }
    text = bits.join('');
  }
  return text;
}

/**
 * take a list of users and draw a whois online list
 */
Drupal.chatroom.chat.updateChatUsers = function(chatUsers) {
  var chatUserList = $('#chatroom-online');
  if (chatUserList.length > 0) {
    // loop through the user list
    for (var i = 0; i < Drupal.settings.chatroom.chatUsers.length; i++) {
      deleteFlag = true;
      for (var j = 0; j < chatUsers.length; j++) {
        // if the user in the browser list is in the list from the server
        if (Drupal.settings.chatroom.chatUsers[i].guestId == chatUsers[j].guestId) {
          // then we don't want to delete them
          deleteFlag = false;
          // if their status in the browser is different than the update
          if (Drupal.settings.chatroom.chatUsers[i].away != chatUsers[j].away) {
            if (chatUsers[j].away) {
              // set them as away onscreen
              $('#' + chatUsers[j].guestId).addClass('chatroom-user-away');
              if (!chatUsers[j].self) {
                Drupal.chatroom.chat.writeSystemMsg(chatUsers[j].user, 'away');
              }
            }
            else {
              // set them as back on screen
              $('#' + chatUsers[j].guestId).removeClass('chatroom-user-away');
              if (!chatUsers[j].self) {
                Drupal.chatroom.chat.writeSystemMsg(chatUsers[j].user, 'back');
              }
            }
            // update the browser list away setting
            Drupal.settings.chatroom.chatUsers[i].away = chatUsers[j].away;
          }
          chatUsers.splice(j, 1);
        }
      }
      if (deleteFlag) {
        // if the user to delete is the current user, kick them out
        if (Drupal.settings.chatroom.chatUsers[i].self) {
          clearInterval(Drupal.chatroom.chat.updates);
          window.location = Drupal.settings.chatroom.kickUrl + Drupal.settings.chatroom.chatId;
          return;
        }
        $('#'+ Drupal.settings.chatroom.chatUsers[i].guestId).remove();
        Drupal.chatroom.chat.writeSystemMsg(Drupal.settings.chatroom.chatUsers[i].user, 'leave');
        Drupal.settings.chatroom.chatUsers.splice(i, 1);
      }
    }
    // add the users who are not in the browser list, but were in the update list
    for (var i = 0; i < chatUsers.length; i++) {
      Drupal.settings.chatroom.chatUsers.push(chatUsers[i]);
      var userInfo = $('<a>');
      userInfo.append(chatUsers[i].user);
      userInfo.attr('href', 'javascript:Drupal.chatroom.chat.selectUser("'+ chatUsers[i].user +'")');
      userInfo.css('color', Drupal.chatroom.chat.getUserColour(chatUsers[i].user));
      userInfo.css('fontWeight', 'bold');
      var li = $('<li>');
      li.attr('id', chatUsers[i].guestId);
      li.append(userInfo);
      if (chatUsers[i].away) {
        li.addClass('chatroom-user-away');
      }
      chatUserList.append(li);
      if (Drupal.settings.chatroom.updateCount > 10) {
        Drupal.chatroom.chat.writeSystemMsg(chatUsers[i].user, 'join');
      }
    }
  }
}

/**
 * Select a user to send a private message.
 */
Drupal.chatroom.chat.selectUser = function(userName) {
  var input = $('#chatroom-msg-input');
  input.val('/msg '+ userName + ' '+ input.val());
  input[0].focus();
  input[0].selectionStart = input.val().length;
}

/**
 * Write a system message to the chat.
 */
Drupal.chatroom.chat.writeSystemMsg = function(msgText, type) {
  switch (type) {
    case 'join':
      var msgClass = 'chatroom-system-join';
      var sysMsg = Drupal.settings.chatroom.joinMessage;
      if ($('#chatroom-user-alert').length > 0 && $('#chatroom-user-alert').attr('checked')) {
        Drupal.chatroom.soundManager.play('user');
        if (Drupal.settings.chatroom.hasfocus == false) {
          Drupal.settings.chatroom.newMsgInfo = msgText + sysMsg;
          clearInterval(Drupal.settings.chatroom.warnInterval);
          Drupal.settings.chatroom.warnInterval = setInterval("Drupal.chatroom.chat.warnNewMsgLoop()", 1500);
        }
      }
      break;
    case 'leave':
      var msgClass = 'chatroom-system-leave';
      var sysMsg = Drupal.settings.chatroom.leaveMessage;
        break;
    case 'away':
      var msgClass = 'chatroom-system-away';
      var sysMsg = Drupal.settings.chatroom.awayMessage;
      break;
    case 'back':
      var msgClass = 'chatroom-system-back';
      var sysMsg = Drupal.settings.chatroom.backMessage;
      break;
  }
  var msgSpan = $('<span>');
  msgSpan.append(msgText);
  msgSpan.addClass(msgClass);

  var p = $('<p>');
  p.append('<html>* </html>');
  p.append(msgSpan);
  p.append('<html>'+ sysMsg +'</html>');
  p.addClass('chatroom-system-msg');

  var msgBoard = $('#chatroom-board');
  msgBoard.append(p);
  msgBoard[0].scrollTop = msgBoard[0].scrollHeight;

  Drupal.settings.chatroom.lastUser = '';
}

/**
 * function that controls sets/clears Drupal.chatroom.chat.writeTime()'s timeout
 */
Drupal.chatroom.chat.setWriteTime = function() {
  if (Drupal.settings.chatroom.writeMsgTimeId) {
    clearTimeout(Drupal.settings.chatroom.writeMsgTimeId);
  }
  Drupal.settings.chatroom.writeMsgTimeId = setTimeout('Drupal.chatroom.chat.writeTime()', Drupal.settings.chatroom.idleInterval);
}

/**
 * function that writes a time to board when idle
 */
Drupal.chatroom.chat.writeTime = function() {
  if (Drupal.settings.chatroom.lastMsgTime != undefined) {
    var msgBoard = $('#chatroom-board');
    var p = $('<p>');
    p.append(Drupal.settings.chatroom.lastMsgTime);
    p.addClass('chatroom-time-msg');
    msgBoard.append(p);
    msgBoard[0].scrollTop = msgBoard[0].scrollHeight;
    Drupal.settings.chatroom.lastUser = '';
    clearInterval(Drupal.settings.chatroom.warnInterval);
    document.title = Drupal.settings.chatroom.pageTitle;
  }
}

/**
 * gets updates from the server for this chat
 */
Drupal.chatroom.chat.getUpdates = function() {
  if (Drupal.settings.chatroom.skipUpdate) {
    Drupal.settings.chatroom.skipUpdate = false;
    return;
  }
  Drupal.settings.chatroom.updateCount++;
  $.post(Drupal.chatroom.chat.getUrl('read'), Drupal.chatroom.chat.prepareMsg({}), Drupal.chatroom.chat.msgCallback);
}

/**
 * update last seen message id
 */
Drupal.chatroom.chat.updateLastMsg = function(msgId) {
  if (msgId > Drupal.settings.chatroom.lastMsgId) {
    Drupal.settings.chatroom.lastMsgId = msgId;
    return true;
  }
  return false;
}

/**
 * Get the colour for a user.
 */
Drupal.chatroom.chat.getUserColour = function(user) {
  for (var i = 0; i < Drupal.settings.chatroom.chatUsers.length; i++) {
    if (Drupal.settings.chatroom.chatUsers[i].user == user) {
      if (Drupal.settings.chatroom.chatUsers[i].colour == undefined) {
        Drupal.settings.chatroom.chatUsers[i].colour = '#000000';
        while (1) {
          var j = Math.round((Drupal.settings.chatroom.userColours.length - 1) * Math.random());
          if (Drupal.settings.chatroom.userColours[j].unUsed) {
            Drupal.settings.chatroom.userColours[j].unUsed = false;
            Drupal.settings.chatroom.chatUsers[i].colour = Drupal.settings.chatroom.userColours[j].colour;
            break;
          }
        }
      }
      return Drupal.settings.chatroom.chatUsers[i].colour;
    }
  }
}

/**
 * Set colours for users.
 */
Drupal.chatroom.chat.setUserColours = function() {
  if ($('#chatroom-online').length > 0) {
    for (var i = 0; i < Drupal.settings.chatroom.chatUsers.length; i++) {
      var userInfo = $('a', $('#'+ Drupal.settings.chatroom.chatUsers[i].guestId));
      userInfo.css('color', Drupal.chatroom.chat.getUserColour(Drupal.settings.chatroom.chatUsers[i].user));
      userInfo.css('fontWeight', 'bold');
    }
  }
}

/**
 * onclick smileys insertion
 */
Drupal.chatroom.chat.smileyInsert = function(acronym) {
  var msgInput = $('#chatroom-msg-input');
  var text = msgInput.val();
  msgInput.val(text +' '+ acronym);
  msgInput[0].focus();
  msgInput[0].selectionStart = msgInput.val().length;
}

/**
 * toggle away status
 */
Drupal.chatroom.chat.setAway = function(obj) {
  if (obj.checked) {
    var msg = Drupal.chatroom.chat.getCommandMsg('/away');
  }
  else {
    var msg = Drupal.chatroom.chat.getCommandMsg('/back');
  }
  $.post(Drupal.chatroom.chat.getUrl('write'), Drupal.chatroom.chat.prepareMsg(msg), Drupal.chatroom.chat.msgCallback);
}

/**
 * Toggle message alert status.
 */
Drupal.chatroom.chat.setMsgAlerts = function(obj) {
  if ($(obj).attr('checked')) {
    Drupal.settings.chatroom.msgAlerts = true;
  }
  else {
    Drupal.settings.chatroom.msgAlerts = false;
  }
}

Drupal.chatroom.chat.warnNewMsgLoop = function() {
  if (document.title == Drupal.settings.chatroom.pageTitle) {
    document.title = Drupal.settings.chatroom.newMsgInfo;
  }
  else {
    document.title = Drupal.settings.chatroom.pageTitle;
  }
}

// Global Killswitch
if (Drupal.jsEnabled) {
  $(document).ready(Drupal.chatroom.chat.addEvents);
}

