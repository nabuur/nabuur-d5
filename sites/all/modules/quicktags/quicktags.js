var edButtons = new Array();
var edOpenTags = new Array();
var edCanvas = new Array();

function edButton(id, display, tagStart, tagEnd, access, open, location, icon) {
  this.id = id;      // used to name the toolbar button
  this.display = display;    // label on button
  this.tagStart = tagStart;   // open tag
  this.tagEnd = tagEnd;    // close tag
  this.access = access;    // access key
  this.open = open;    // set to -1 if tag does not need to be closed
  this.location = location;
  this.icon = icon;
}

function edAddTag(button) {
  if (edButtons[button].tagEnd != '') {
    edOpenTags[edOpenTags.length] = button;
    document.getElementById(edButtons[button].id).alt = '/' + document.getElementById(edButtons[button].id).alt;
                document.getElementById(edButtons[button].id).style.border = 'solid red 1px';
  }
}

function edRemoveTag(button) {
  for (i = 0; i < edOpenTags.length; i++) {
    if (edOpenTags[i] == button) {
      edOpenTags.splice(i, 1);
      document.getElementById(edButtons[button].id).alt =     document.getElementById(edButtons[button].id).alt.replace('/', '');
                        document.getElementById(edButtons[button].id).style.border = '';
    }
  }
}

function edCheckOpenTags(button) {
  var tag = 0;
  for (i = 0; i < edOpenTags.length; i++) {
    if (edOpenTags[i] == button) {
      tag++;
    }
  }
  if (tag > 0) {
    return true; // tag found
  }
  else {
    return false; // tag not found
  }
}

function edCloseAllTags(id) {
  var count = edOpenTags.length;
  for (o = 0; o < count; o++) {
    edInsertTag(edCanvas[id], edOpenTags[edOpenTags.length - 1]);
  }
}

// insertion code
function edInsertTag(myField, i) {
  //IE support
  if (document.selection) {
    myField.focus();
      sel = document.selection.createRange();
    if (sel.text.length > 0) {
      sel.text = edButtons[i].tagStart + sel.text + edButtons[i].tagEnd;
    }
    else {
      if (!edCheckOpenTags(i) || edButtons[i].tagEnd == '') {
        sel.text = edButtons[i].tagStart;
        edAddTag(i);
      }
      else {
        sel.text = edButtons[i].tagEnd;
        edRemoveTag(i);
      }
    }
    myField.focus();
  }
  //MOZILLA/NETSCAPE support
  else if (myField.selectionStart || myField.selectionStart == '0') {
    var startPos = myField.selectionStart;
    var endPos = myField.selectionEnd;
    var cursorPos = endPos;
    if (startPos != endPos) {
      myField.value = myField.value.substring(0, startPos)
                    + edButtons[i].tagStart
                    + myField.value.substring(startPos, endPos)
                    + edButtons[i].tagEnd
                    + myField.value.substring(endPos, myField.value.length);
      cursorPos += edButtons[i].tagStart.length + edButtons[i].tagEnd.length;
    }
    else {
      if (!edCheckOpenTags(i) || edButtons[i].tagEnd == '') {
        myField.value = myField.value.substring(0, startPos)
                      + edButtons[i].tagStart
                      + myField.value.substring(endPos, myField.value.length);
        edAddTag(i);
        cursorPos = startPos + edButtons[i].tagStart.length;
      }
      else {
        myField.value = myField.value.substring(0, startPos)
                      + edButtons[i].tagEnd
                      + myField.value.substring(endPos, myField.value.length);
        edRemoveTag(i);
        cursorPos = startPos + edButtons[i].tagEnd.length;
      }
    }
    myField.focus();
    myField.selectionStart = cursorPos;
    myField.selectionEnd = cursorPos;
  }
  else {
    if (!edCheckOpenTags(i) || edButtons[i].tagEnd == '') {
      myField.value += edButtons[i].tagStart;
      edAddTag(i);
    }
    else {
      myField.value += edButtons[i].tagEnd;
      edRemoveTag(i);
    }
    myField.focus();
  }
}

function edInsertContent(myField, myValue) {
  //IE support
  if (document.selection) {
    myField.focus();
    sel = document.selection.createRange();
    sel.text = myValue;
    myField.focus();
  }
  //MOZILLA/NETSCAPE support
  else if (myField.selectionStart || myField.selectionStart == '0') {
    var startPos = myField.selectionStart;
    var endPos = myField.selectionEnd;
    myField.value = myField.value.substring(0, startPos)
                  + myValue
                      + myField.value.substring(endPos, myField.value.length);
    myField.focus();
    myField.selectionStart = startPos + myValue.length;
    myField.selectionEnd = startPos + myValue.length;
  } else {
    myField.value += myValue;
    myField.focus();
  }
}


function edShowButton(button, i, id) {
  if (button.location) {
    document.write('<a href="javascript:'+button.location+'(edCanvas[' + id + '], ' + i + ');" accesskey="' + button.access + '"><img src="' + button.icon + '" id="' + button.id + '" class="ed_button" alt="' + button.display + '" title="' + button.display + ' ('+ button.access +')"/></a>');
  }
  else {
    document.write('<a href="javascript:edInsertTag(edCanvas['+ id +'], '+ i +');" accesskey="' + button.access + '"><img src="' + button.icon + '" id="' + button.id + '" class="ed_button" alt="' + button.display + '" title="' + button.display + ' ('+ button.access +')"/></a>');
  }
}
