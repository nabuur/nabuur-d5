<?php
// $Id: zzz_custom_url.inc,v 1.2 2008/05/30 18:35:37 aaron Exp $

function _emaudio_zzz_custom_url_default_types() {
  return array('mp3', 'wav', 'rm');
}

function emaudio_zzz_custom_url_info() {
  $name = t('Custom URL');
  $features = array(
    array(t('Thumbnails'), t('No'), ''),
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS attachment'), t('Yes'), ''),
  );
  return array(
    'provider' => 'zzz_custom_url',
    'name' => $name,
    'url' => '',
    'settings_description' => t('These settings specifically affect audio media displayed from custom URL\'s. When a field uses a URL it determines to be a link directly to an audio file, it will embed that file into the content.'),
    'supported_features' => $features,
    'weight' => 9,
  );
}

function emaudio_zzz_custom_url_settings() {
  $options = array(
    'mp3' => t('MP3'),
    'wav' => t('Wav'),
    'rm' => t('Real Media (rm)'),
  );
  $form = array();
  $form['emaudio_zzz_custom_url_supported_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Supported Types'),
    '#options' => $options,
    '#default_value' => variable_get('emaudio_zzz_custom_url_supported_types', _emaudio_zzz_custom_url_default_types()),
    '#description' => t('Select the audio types you wish to support. When a custom url with that type is entered into an embedded audio field, it will be parsed and displayed appropriately. If a type is not supported, then it will be ignored.'),
  );
  return $form;
}

function _emaudio_zzz_custom_url_implode_types() {
  return implode('|', variable_get('emaudio_zzz_custom_url_supported_types', _emaudio_zzz_custom_url_default_types()));
}

function emaudio_zzz_custom_url_extract($embed = '') {
  $types = _emaudio_zzz_custom_url_implode_types();
  if (preg_match('@\.('. $types .')@i', $embed, $matches)) {
    return $embed;
  }
  return false;
}

function emaudio_zzz_custom_url_data($field, $item) {
  $data = array();
  // adding the version control
  $data['emaudio_zzz_custom_url_data_version'] = 1;

  // attempt to get info from headers
  $response = emfield_request_header('zzz_custom_url', $item['embed']);

  if ($response->code == 200) {
    $data['url'] = $item['embed'];
    $data['size'] = $response->headers['Content-Length'];
    $data['mime'] = $response->headers['Content-Type'];
  }
  // @todo replace ['type'] with converted mime info if available
  $types = _emaudio_zzz_custom_url_implode_types();
  $regex = '@\.('. $types .')@i';
  if (preg_match($regex, $item['embed'], $matches)) {
    $data['type'] = $matches[1];
  }
  return $data;
}

/**
 * hook emfield_PROVIDER_rss
 */
function emaudio_zzz_custom_url_rss($item, $teaser = NULL) {
  if ($item['value']) {
    if ($item['data']['emaudio_zzz_custom_url_data_version'] >= 1) {
      $data = $item['data'];
    }
    else {
      $data = emaudio_zzz_custom_url_data(NULL, $item);
    }

    $file = array();
    if ($data['size'] > 0) {
      $file['filepath'] = $data['url'];
      $file['filesize'] = $data['size'];
      $file['filemime'] = $data['mime'];
    }

    return $file;
  }
}


function emaudio_zzz_custom_url_embedded_link($audio_code) {
  return $audio_code;
}

function theme_emaudio_zzz_custom_url_embedded_audio($type, $url, $width, $height, $autoplay = false, $field = NULL, $item = NULL, $node = NULL, $options = array()) {
  static $count;
  if ($url) {
    $count++;
    $title = $options['title'] ? $options['title'] : t('Download audio');
    $class = $options['class'] ? $options['class'] : 'emaudio-zzz-custom-url-embedded-audio media';
    $id = $options['id'] ? $options['id'] : "$class-$count";
    switch ($type) {
      case 'mp3':
      case 'wav':
      case 'rm':
        return l($title, $url, array('id' => $id, 'class' => $class));
    }
  }
}

function emaudio_zzz_custom_url_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return '';
}

function emaudio_zzz_custom_url_audio($code, $width, $height, $field, $item, $autoplay, $node = NULL) {
  $type = $item['data']['type'];
  $output = theme('emaudio_zzz_custom_url_embedded_audio', $type, $code, $width, $height, $autoplay, $field, $item, $node, $options);
  return $output;
}

function emaudio_zzz_custom_url_preview($code, $width, $height, $field, $item, $autoplay, $node = NULL) {
  $type = $item['data']['type'];
  $output = theme('emaudio_zzz_custom_url_embedded_audio', $type, $code, $width, $height, $autoplay, $field, $item, $node, $options);
  return $output;
}
