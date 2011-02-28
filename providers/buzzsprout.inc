<?php

/**
 * @file
 *  This is an buzzsprout provider include file for Embedded Media audio.
 *  Use this as a base for creating new provider files.
 *
 *  When using this, first make the following global replacements:
 *    * Replace buzzsprout with the name of your provider in all caps.
 *    * Replace buzzsprout with the name of your provider in all lower case.
 *    * Replace buzzsprout with the name (to be translated) of your provider in
 *        uppercase.
 *
 *  You then need to go through each function and modify according to the
 *  requirements of your provider's API.
 */

/**
 *  This is the main URL for your provider.
 */
define('EMAUDIO_BUZZSPROUT_MAIN_URL', 'http://buzzsprout.org');

/**
 *  This is the URL to the API of your provider, if this exists.
 */
define('EMAUDIO_BUZZSPROUT_API_URL', 'http://f.muneef.in/500.php?q=URL');

/**
 *  This defines the version of the content data array that we serialize
 *  in emaudio_buzzsprout_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMAUDIO_BUZZSPROUT_DATA_VERSION', 1);

/**
 * hook emaudio_PROVIDER_info
 * This returns information relevant to a specific 3rd party audio provider.
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
function emaudio_buzzsprout_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => 'buzzsprout',
    'name' => t('buzzsprout'),
    'url' => EMAUDIO_BUZZSPROUT_MAIN_URL,
    'settings_description' => t('These settings specifically affect audio played from !buzzsprout. You can also read more about its !api.', array('!buzzsprout' => l(t('buzzsprout.com'), EMAUDIO_buzzsprout_MAIN_URL), '!api' => l(t("developer's API"), EMAUDIO_buzzsprout_API_URL))),
    'supported_features' => $features,
  );
}

/**
 *  hook emaudio_PROVIDER_settings
 *  This should return a subform to be added to the emaudio_settings() admin
 *  settings page.
 *
 *  Note that a form field set will already be provided at $form['buzzsprout'],
 *  so if you want specific provider settings within that field set, you should
 *  add the elements to that form array element.
 */
function emaudio_buzzsprout_settings() {
  // We'll add a field set of player options here. You may add other options
  // to this element, or remove the field set entirely if there are no
  // user-configurable options allowed by the buzzsprout provider.
  $form = array();
  $form['buzzsprout']['media_buzzsprout_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Buzzsprout Userid'),
    '#description' => t('Buzzsprout Userid'),
    '#default_value' => variable_get('media_buzzsprout_api_key', ''),
    '#size' => 40,
    '#maxlength' => 40,
  );
  $form['buzzsprout']['media_buzzsprout_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Buzzsprout Username'),
    '#default_value' => variable_get('media_buzzsprout_username', ''),
    '#size' => 40,
    '#maxlength' => 40,
  );
  return $form;
}

/**
 *  hook emaudio_PROVIDER_extract
 *
 *  This is called to extract the audio code from a pasted URL or embed code.
 *
 *  We'll be passed a URL or the embed code from a audio when an editor pastes
 *  that in the field's textfield. We'll need to either pass back an array of
 *  regex expressions to match, or do the matching ourselves and return the
 *  resulting audio code.
 *
 *  @param $parse
 *    An optional string with the pasted URL or embed code.
 *  @return
 *    Either an array of regex expressions to be tested, or a string with the
 *    audio code to be used. If the hook tests the code itself, it should
 *    return either the string of the audio code (if matched), or an empty
 *    array. Otherwise, the calling function will handle testing the embed code
 *    against each regex string in the returned array.
 */
function emaudio_buzzsprout_extract($parse = '') {
  // We'll simply return an array of regular expressions for Embedded Media
  // Field to handle for us.
  
  
  return array(
	'@buzzsprout\.com/[0-9]+/(.+)@i',
  );
}

/**
 *  Implement hook EMAUDIO_PROVIDER_data_version().
 */
function emaudio_buzzsprout_data_version() {
  return EMAUDIO_BUZZSPROUT_DATA_VERSION;
}


/**
 *  hook emaudio_PROVIDER_data
 *
 *  Provides an array to be serialised and made available with $item elsewhere.
 *
 *  This data can be used to store any extraneous information available
 *  specifically to the buzzsprout provider.
 */
function emaudio_buzzsprout_data($field, $item) {
  $data = array();
  $apikey = variable_get('buzzsprout_api_key', '');
  
  // inbuilt fetch.
  $data=buzzsprout_fetcher($item['value']);
  // use this to fetch xml
  // $data = emfield_request_xml('buzzsprout', 'http://f.muneef.in/500.php?q=http://jupiqon.buzzsprout.com/'.$item['value'], array(), FALSE, FALSE, $item['value']);
  
  $data['audio_id'] = $item['value'];
  $data['emaudio_buzzsprout_version'] = EMAUDIO_BUZZSPROUT_DATA_VERSION;
  return $data;
}


/**
 * hook emaudio_PROVIDER_embedded_link($code)
 * returns a link to view the audio at the provider's site.
 *  @param $code
 *    The string containing the audio code.
 *  @return
 *    A string containing the URL to listen to the audio at the original provider's site.
 */
function emaudio_buzzsprout_embedded_link($code) {
  $apikey = variable_get('buzzsprout_api_key', '');
  $username = variable_get('buzzsprout_username', '');
  return 'http://'.$username.'.buzzsprout.com/'.$apikey.'/'. $code;
}

/**
 * The embedded flash displaying the buzzsprout audio.
 */
function theme_emaudio_buzzsprout_flash($item, $code, $width, $height, $autoplay, $options) {
	$output = '';  
  
	drupal_add_js(drupal_get_path('module', 'media_buzzsprout').'/js/audio-player.js');
	$x2='AudioPlayer.setup("/'.drupal_get_path('module', 'media_buzzsprout').'/js/player.swf", {  width: 290, initialvolume: 100, transparentpagebg: "yes", left: "000000", lefticon: "FFFFFF" });';
	drupal_add_js($x2,'inline');
	if(isset($item['data']['OEMBED']['FILE'][0])){
		$output='<p id="audioplayer_1">'.l('Download this Audio ',$item['data']['OEMBED']['FILE'][0]).'</p>  
		 <script type="text/javascript">  
		 AudioPlayer.embed("audioplayer_1", {soundFile: "'.$item['data']['OEMBED']['FILE'][0].'"});  
		</script>';
	}
    return $output;
}

/*
 *  hook emaudio_PROVIDER_audio
 *  This actually displays the full/normal-sized audio we want, usually on the
 *  default page view.
 *  @param $embed
 *  @param $width
 *    The width to display the audio.
 *  @param $height
 *    The height to display the audio.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded audio.
 */
function emaudio_buzzsprout_audio($code, $width, $height, $field, $item, &$node, $autoplay, $options = array()) {
 $output = theme('emaudio_buzzsprout_flash', $item, $code, $width, $height, $autoplay, $options);
 return $output;
}

/**
 *  hook emaudio_PROVIDER_preview
 *
 *  This actually displays the preview-sized audio we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The audio code for the audio to embed.
 *  @param $width
 *    The width to display the audio.
 *  @param $height
 *    The height to display the audio.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded audio.
 */
function emaudio_buzzsprout_preview($code, $width, $height, $field, $item, &$node, $autoplay, $options = array()) {
  $output = theme('emaudio_buzzsprout_flash', $item, $code, $width, $height, $autoplay, $options);
  return $output;
}

/**
 *  Implementation of hook_emfield_subtheme.
 *  This returns any theme functions defined by this provider.
 */
function emaudio_buzzsprout_emfield_subtheme() {
  $themes = array(
    'emaudio_buzzsprout_flash'  => array(
      'arguments' => array('code' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL, 'options' =>NULL,),
      'file' => 'providers/buzzsprout.inc',
      'path' => drupal_get_path('module', 'media_buzzsprout'),
    )
  );
  return $themes;
}

	function get_external_css($file){
		$h1tags = preg_match_all('/(rel=)(".*") href=(".*")/im',$file,$patterns);
		$res = array();
		array_push($res,$patterns);
		array_push($res,count($patterns[2]));
		return $res; 
	} 
	
	function buzzsprout_fetcher($buzz_item_link)
	{
		$apikey = variable_get('buzzsprout_api_key', '');
		$username = variable_get('buzzsprout_username', '');
		
		$url=$buzz_item_link;
		$request = drupal_http_request('http://'.$username.'.buzzsprout.com/'.$apikey.'/'.$url);
		$data = $request->data;

		$rels=get_external_css($data);
		
		foreach($rels[0][0] as $x){
			if(preg_match_all("/audio_src/",$x,$mm)){
				preg_match_all('/href=\"(.*)\"/',$x,$mm);
				
				$file['src']=$mm[1][0];				
				$titre = eregi("<title>(.*)</title>",$data,$regs);
				$titl=$regs[1];
				$file['title']=$titl;
				return $file;
			}
		}
	}