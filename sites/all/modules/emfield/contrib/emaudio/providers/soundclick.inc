<?php
// $Id: soundclick.inc,v 1.1 2008/05/30 18:02:27 aaron Exp $
define('EMAUDIO_SOUNDCLICK_MAIN_URL', 'http://soundclick.com/');
/**
 * Implementation of hook emaudio_soundclick_info().
 * This returns information relevant to a specific 3rd party audio provider.
 * @return
 *   An array of strings requested by various admin and other forms.
 *   'name' => The translated name of the provider.
 *   'url' => The url to the main page for the provider.
 *   'settings_description' => A description of the provider that will be posted in the admin settings form.
 *   'supported_features' => An array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */
function emaudio_soundclick_info() {
  $name = t('SoundClick');
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => 'soundclick',
    'name' => $name,
    'url' => EMAUDIO_SOUNDCLICK_MAIN_URL,
    'settings_description' => t('These settings specifically affect audio podcasts displayed from !soundclick.', array('!soundclick' => l($name, EMAUDIO_SOUNDCLICK_MAIN_URL, array('target' => '_blank')))),
    'supported_features' => $features,
  );
}

/**
 * Implementation of hook emaudio_soundclick_settings().
 * This should return a subform to be added to the emaudio_settings() admin settings page.
 * Note that a form field will already be provided, at $form['soundclick'] (such as $form['podomatic']).
 * So if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emaudio_soundclick_settings() {
  $form = array();
  return $form;
}

/**
 * Implementation of hook emaudio_soundclick_extract().
 * This is called to extract the video code from a pasted URL or embed code.
 * @param $embed an optional string with the pasted URL or embed code.
 * @return either an array of regex expressions to be tested, or a string with the audio code to be used.
 *   If the hook tests the code itself, it should return either the string of the audio code (if matched), or an empty array.
 *   Otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emaudio_soundclick_extract($embed = '') {
  // http://www.soundclick.com/bands/default.cfm?bandid=838201&content=widgets&style=player&id=3
  return array(
    '@soundclick\.com/bands/default\.cfm\?bandid=([^\&]+)@i',
  );
}

/**
 * The embedded flash displaying the soundclick audio.
 */
function theme_emaudio_soundclick_flash($embed, $width, $height, $autoplay) {
  static $count;
  if ($embed) {
    $autoplay = $autoplay ? 1 : 0;
    $id = 'emaudio-soundclick-'. ++$count;
    $output .= "
      <object classid=\"clsid:d27cdb6e-ae6d-11cf-96b8-444553540000\" codebase=\"http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,47,0\" width=\"$width\" height=\"$height\" id=\"$id\" align=\"center\">
      <param name=\"quality\" value=\"high\" />
      <param name=\"width\" value=\"$width\" />
      <param name=\"height\" value=\"$height\" />
      <param name=\"name\" value=\"MP3PlayerVIP\" />
      <param name=\"allowScriptAccess\" value=\"always\" />
      <param name=\"wmode\" value=\"transparent\" />
      <embed src=\"http://www.soundclick.com/widgets/creatives/mp3PlayerVIP.swf\"
      quality=\"high\" width=\"$width\" height=\"$height\"
      name=\"MP3PlayerVIP\" align=\"middle\"
      allowScriptAccess=\"always\" wmode=\"transparent\"
      type=\"application/x-shockwave-flash\" flashvars=\"bandid=$embed&ext=1&autoplay=$autoplay\" pluginspage=\"http://www.macromedia.com/go/getflashplayer\" />
      </object>
    \n";
  }
  return $output;
}

/**
 * Implementation of hook emaudio_soundclick_thumbnail().
 * Returns the external url for a thumbnail of a specific audio.
 * TODO: make the args: ($embed, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things.
 *  @param $field the field of the requesting node.
 *  @param $item the actual content of the field from the requesting node.
 *  @return a URL pointing to the thumbnail.
 */
function emaudio_soundclick_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return $tn;
}

/**
 * Implementation of hook emaudio_soundclick_audio().
 * This actually displays the full/normal-sized video we want, usually on the default page view.
 *  @param $embed the video code for the audio to embed.
 *  @param $width the width to display the audio.
 *  @param $height the height to display the audio.
 *  @param $field the field info from the requesting node.
 *  @param $item the actual content from the field.
 *  @return the html of the embedded audio.
 */
function emaudio_soundclick_audio($embed, $width, $height, $field, $item, $autoplay) {
  $output = theme('emaudio_soundclick_flash', $embed, $width, $height, $autoplay);
  return $output;
}

/**
 * Implementation of hook emaudio_soundclick_preview().
 * This actually displays the preview-sized video we want, commonly for the teaser.
 *  @param $embed the video code for the audio to embed.
 *  @param $width the width to display the audio.
 *  @param $height the height to display the audio.
 *  @param $field the field info from the requesting node.
 *  @param $item the actual content from the field.
 *  @return the html of the embedded audio.
 */
function emaudio_soundclick_preview($embed, $width, $height, $field, $item, $autoplay) {
  $output = theme('emaudio_soundclick_flash', $embed, $width, $height, $autoplay);
  return $output;
}

