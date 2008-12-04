<?php
// $Id: update_term_images.php,v 1.1.10.2 2008/03/23 15:24:01 nancyw Exp $
// This script allows an administrator to remotely convert from using the
// node_image module to using the taxonomy_image module.
// Written by John Vandenberg <zeroj at null.net>

$access_check = 1;

include "includes/bootstrap.inc";
include "includes/common.inc";

$term_dir = variable_get("node_image_term_dir", "images/term");

function migrate_term_image_get_tid($name) {
  if ($index = strpos($name, '_full')) {
    return substr($name, 0, $index);
  }
  else {
    return substr($name, 0, strpos($name, '_summ'));
  }
}

function list_node_image_terms() {
  global $term_dir;
  $files = file_scan_directory($term_dir, '.*_(full|summ).*');
  if (!count($files)) {
    print 'No node_images for terms; update script does not support node images.';
    return;
  }
  $header = array(t('Term'), t('Image'), t('Migrate'), t('Delete'));
  foreach ($files as $image) {
    $image->tid = migrate_term_image_get_tid($image->name);

    list($image->width, $image->height) = getimagesize($image->filename);
    if ($image->height > 100) {
      $image->height = $image->height / 2;
      $image->width = $image->width / 2;
    }
    $img = "<img src='$image->filename ' width='$image->width' height='$image->height' />";
    $term =  taxonomy_get_term($image->tid);
    $t_i_image = db_fetch_object(db_query('SELECT path FROM {term_image} WHERE tid = %d', $image->tid));
    if ($t_i_image) $term->has_image = true;
    unset($t_i_image);
    $item = $term->name .'($image->name)';
    $migrate = form_checkbox(NULL, "migrate][". $image->name, 1, $term->has_image ? 0 : 1);
    $delete = form_checkbox(NULL, "delete][". $image->name, 1, $term->has_image ? 1 : 0);
 
    $rows[] = array('data' => $item, $img, $migrate, $delete);
  }
  $table = theme('table', $header, $rows);
  $table .= '<em>Checked rows do not have an existing taxonomy_image entry</em><br/>';
  $form = form($table . form_submit('Migrate'));
  print $form;
}

function migrate() {
  global $term_dir;
  $completed = array();

  $names = $_POST['edit']['migrate'];
  foreach ($names as $name => $status) {
    $files = file_scan_directory($term_dir, $name);
    if (count($files) != 1) {
      print count($files) ." files match the name: $name";
      exit();
    }
    $image = array_pop($files);
    $image->filename_orig = $image->filename;
    if ($status == 1) {
      $image->tid = migrate_term_image_get_tid($image->name);
      if (!taxonomy_get_term($image->tid)) {
        print "cant find the tid: $tid";
        exit();
      }
      $t_i_image = db_fetch_object(db_query('SELECT path FROM {term_image} WHERE tid = %d', $image->tid));
      if ($t_i_image) $term->has_image = true;

      if ($term->has_image) taxonomy_image_delete($image->tid);

      if (file_copy($image->filename) != 1) {
        print "couldnt copy file: $image->filename to new location";
        exit();
      }

      db_query("INSERT INTO {term_image} (tid, path) VALUES (%d, '%s')", $image->tid, $image->filename);
      $completed[] = $image;
    }
    if ($_POST['edit']['delete'][$name] == 1) {
      file_delete($image->filename_orig);
      $deleted[] = $image;
    }
  } 
  if ($c = count($completed)) {
    print "Updated $c terms";
  }
  else {
    print "No terms updated";
  }
  if ($c = count($deleted)) {
    print "Deleted $c node_image(s)";
  }
  else {
    print "No images deleted";
  }
}

if (isset($_POST["op"])) {
  // Access check:
  if (($access_check == 0) || ($user->uid == 1)) {
    print "Migrating ... <br/>";
    migrate();
  }
  else {
    print "Access denied.  You are not authorized to access to this page.  Please log in as the user with user ID #1. If you cannot log-in, you will have to edit this file";
  }
}

list_node_image_terms();
