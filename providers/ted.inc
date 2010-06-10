<?php
// $Id$

/**
 *  This is the main URL for your provider.
 * @todo cleanup comments
 * @todo parse default language attributes for the video embed
 */
define('EMVIDEO_TED_MAIN_URL', 'http://ted.com/');
define('EMVIDEO_TED_PLAYER_URL', 'http://video.ted.com/assets/player/swf/EmbedPlayer.swf');
define('EMVIDEO_TED_ID_EMBED_URL'. 'http://www.ted.com/talks/embed/id/');
/**
 *  This is the URL to the API of your provider, if this exists.
 */
//define('EMVIDEO_TED_API_URL', 'http://');

/**
 *  This defines the version of the content data array that we serialize
 *  in emvideo_dostub_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMVIDEO_TED_DATA_VERSION', 1);

/**
 * hook emvideo_PROVIDER_info
 * This returns information relevant to a specific 3rd party video provider.
 *
 * @return
 *   A keyed array of strings requested by various admin and other forms.
 *    'provider' => The machine name of the provider. This must be the same as
 *      the base name of this filename, before the .inc extension.
 *    'name' => The translated name of the provider.
 *    'url' => The url to the main page for the provider.
 *    'settings_description' => A description of the provider that will be
 *      posted in the admin settings form.
 *    'supported_features' => An array of rows describing the state of certain
 *      supported features by the provider. These will be rendered in a table,
 *      with the columns being 'Feature', 'Supported', 'Notes'. In general,
 *      the 'Feature' column will give the name of the feature, 'Supported'
 *      will be Yes or No, and 'Notes' will give an optional description or
 *      caveats to the feature.
 */
function emvideo_ted_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('Yes'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
  );
  return array(
    'provider' => 'ted',
    'name' => t('TED'),
    'url' => EMVIDEO_TED_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from !provider. You can also read more about its !api.', array('!provider' => l(t('dotSUB.com'), EMVIDEO_DOTSUB_MAIN_URL), '!api' => l(t("developer's API"), EMVIDEO_DOTSUB_API_URL))),
    'supported_features' => $features,
  );
}

/**
 *  hook emvideo_PROVIDER_settings
 *  This should return a subform to be added to the emvideo_settings() admin
 *  settings page.
 *
 *  Note that a form field set will already be provided at $form['example'],
 *  so if you want specific provider settings within that field set, you should
 *  add the elements to that form array element.
 */
function emvideo_ted_settings() {
  // We'll add a field set of player options here. You may add other options
  // to this element, or remove the field set entirely if there are no
  // user-configurable options allowed by the example provider.
  $form['ted']['player_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // This is an option to set the video to full screen. You should remove this
  // option if it is not provided by the example provider.
  $form['ted']['player_options']['emvideo_ted_full_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow fullscreen'),
    '#default_value' => variable_get('emvideo_ted_full_screen', 1),
    '#description' => t('Allow users to view video using the entire computer screen.'),
  );

  return $form;
}

/**
 *  hook emvideo_PROVIDER_extract
 *
 *  This is called to extract the video code from a pasted URL or embed code.
 *
 *  We'll be passed a URL or the embed code from a video when an editor pastes
 *  that in the field's textfield. We'll need to either pass back an array of
 *  regex expressions to match, or do the matching ourselves and return the
 *  resulting video code.
 *
 *  @param $parse
 *    An optional string with the pasted URL or embed code.
 *  @return
 *    Either an array of regex expressions to be tested, or a string with the
 *    video code to be used. If the hook tests the code itself, it should
 *    return either the string of the video code (if matched), or an empty
 *    array. Otherwise, the calling function will handle testing the embed code
 *    against each regex string in the returned array.
 */
function emvideo_ted_extract($embed = '') {
  // Here we assume that a URL will be passed in the form of
  // a TED.com embed code
/*/
 [ted id=319] --> http://www.ted.com/talks/embed/id/319
<!--copy and paste-->
<object width="446" height="326">
  <param name="movie" value="http://video.ted.com/assets/player/swf/EmbedPlayer.swf"></param>
  <param name="allowFullScreen" value="true" />
  <param name="allowScriptAccess" value="always"/>
  <param name="wmode" value="transparent"></param>
  <param name="bgColor" value="#ffffff"></param>
  <param name="flashvars" value="vu=http://video.ted.com/talks/dynamic/EvanWilliams_2009-medium.flv&su=http://images.ted.com/images/ted/tedindex/embed-posters/EvanWilliams-2009.embed_thumbnail.jpg&vw=432&vh=240&ap=0&ti=473&introDuration=15330&adDuration=4000&postAdDuration=830&adKeys=talk=evan_williams_on_listening_to_twitter_users;year=2009;theme=speaking_at_ted2009;theme=tales_of_invention;theme=what_s_next_in_tech;theme=words_about_words;theme=not_business_as_usual;event=TED2009;&preAdTag=tconf.ted/embed;tile=1;sz=512x288;" />
  <embed src="http://video.ted.com/assets/player/swf/EmbedPlayer.swf" pluginspace="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" wmode="transparent" bgColor="#ffffff" width="446" height="326" allowFullScreen="true" allowScriptAccess="always" flashvars="vu=http://video.ted.com/talks/dynamic/EvanWilliams_2009-medium.flv&su=http://images.ted.com/images/ted/tedindex/embed-posters/EvanWilliams-2009.embed_thumbnail.jpg&vw=432&vh=240&ap=0&ti=473&introDuration=15330&adDuration=4000&postAdDuration=830&adKeys=talk=evan_williams_on_listening_to_twitter_users;year=2009;theme=speaking_at_ted2009;theme=tales_of_invention;theme=what_s_next_in_tech;theme=words_about_words;theme=not_business_as_usual;event=TED2009;"></embed></object>
//*/
	
	if ($embed && preg_match('@\[ted id=\"([0*9]+)\"\]@i', $embed, $matches)) {
		$result = drupal_http_request(EMVIDEO_TED_ID_EMBED_UR . $matches[1]);
		if ($result->code == 200)
		  $embed = $result->data;
	}
	
	if ($embed && preg_match('@\"vu=([^\"]+)\"@i', $embed, $matches)) {
		return 'vu=' . $matches[1];
	}
  
  return FALSE;
}

/**
 *  hook emvideo_PROVIDER_data
 *
 *  Provides an array to be serialised and made available with $item elsewhere.
 *
 *  This data can be used to store any extraneous information available
 *  specifically to the example provider.
 */

function emvideo_ted_data($field, $item) {
  // Initialize the data array.
  $data = array();
  $data['emvideo_ted_version'] = EMVIDEO_TED_DATA_VERSION;

  // We are using oEmbed to retrieve a standard set of data from the provider.
  // You should change the URL as specified by the example provider.
  // If the example provider does not support oEmbed, you must remove this
  // section entirely, or rewrite it to use their API.
  // See http://oembed.com/ for for information.
//  $xml = emfield_request_xml('example', 'http://www.example.com/api/oembed.xml?url=http%3A//www.example.com/video/'. $item['value'], array(), TRUE, FALSE, $item['value']);
  // This stores a URL to the video's thumbnail.
//    $data['thumbnail'] = $xml['OEMBED']['THUMBNAIL_URL'][0];

  // Gather info about the item's raw flash video.
  // RSS / MRSS feeds with the item would have enough info.
  // Alternatively try getting the minimum from an HTTP get.
  $url = 'http://video.ted.com/assets/player/swf/EmbedPlayer.swf?vu='. $item['value'];
  
  $response = emfield_request_header('dotsub', $url);
  
  if ($response->code == 200) {
    // Don't give the 303 path.
    $data['flash']['url'] = $url;
    $data['flash']['size'] = $response->headers['Content-Length'];
    $data['flash']['mime'] = $response->headers['Content-Type'];
  }

  return $data;
}
//*/

/**
 *  hook emvideo_PROVIDER_rss
 *
 *  This attaches a file to an RSS feed.
 */
function emvideo_ted_rss($item, $teaser = NULL) {
  if ($item['value']) {
    $file['thumbnail']['filepath'] = $item['data']['thumbnail'];

    return $file;
  }
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site.
 *  @param $video_code
 *    The string containing the video to watch.
 *  @return
 *    A string containing the URL to view the video at the original provider's site.
 */
function emvideo_ted_embedded_link($video_code) {
//http://dotsub.com/view/...  
  return 'http://ted.com'. $video_code;
}

/**
 * The embedded flash displaying the dotsub video.
 */
function theme_emvideo_ted_flash($item, $width, $height, $autoplay) {

  $output = '';
  if ($item['embed']) {
    
    $vars = $item['value'];
    //TODO remove redundant(?) $embed_url
  	$embed_url = isset($item['data']['flash']['url']) ? $item['data']['flash']['url'] : EMVIDEO_TED_PLAYER_URL . $vars;
  	$movie_url = EMVIDEO_TED_PLAYER_URL;
    
    $autoplay = $autoplay ? 'true' : 'false';
    $fullscreen = variable_get('emvideo_ted_full_screen', 1) ? 'true' : 'false';
    
    $output = <<<EOD
<object width="$width" height="$height">
  <param name="movie" value="$movie_url"></param>
  <param name="allowFullScreen" value="$fullscreen" />
  <param name="allowScriptAccess" value="always"/>
  <param name="wmode" value="transparent"></param>
  <param name="bgColor" value="#ffffff"></param>
  <param name="flashvars" value="$vars" />
  <embed src="$movie_url" pluginspace="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" wmode="transparent" bgColor="#ffffff" width="$width" height="$height" allowFullScreen="$fullscreen" allowScriptAccess="always" flashvars="$vars"></embed>
</object>
EOD;
  }
  return $output;
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * Returns the external url for a thumbnail of a specific video.
 *  @param $field
 *    The field of the requesting node.
 *  @param $item
 *    The actual content of the field from the requesting node.
 *  @return
 *    A URL pointing to the thumbnail.
 */
function emvideo_ted_thumbnail($field, $item, $formatter, $node, $width, $height) {
  // In this demonstration, we previously retrieved a thumbnail using oEmbed
  // during the data hook.
  return $data['thumbnail'];
}

/**
 *  hook emvideo_PROVIDER_video
 *  This actually displays the full/normal-sized video we want, usually on the
 *  default page view.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_ted_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_ted_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 *  hook emvideo_PROVIDER_video
 *
 *  This actually displays the preview-sized video we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_ted_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_ted_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 *  Implementation of hook_emfield_subtheme.
 *  This returns any theme functions defined by this provider.
 */
function emvideo_ted_emfield_subtheme() {
    $themes = array(
        'emvideo_ted_flash'  => array(
            'arguments' => array('item' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
            'file' => 'providers/ted.inc',
            'path' => drupal_get_path('module', 'media_ted'),
        )
    );
    return $themes;
}
