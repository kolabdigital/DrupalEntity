<?php

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions for existing leads.
 */
function kolab_lead_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $query = db_select('lead', 'l')->fields('l', array('first_name', 'surname', 'lid'));
    //$query->addExpression("CONCAT(l.first_name, ' ', l.surname)", 'full_name');
    $or = db_or();
    $or->condition('l.first_name', db_like($string) . '%', 'LIKE');
    $or->condition('l.surname', db_like($string) . '%', 'LIKE');
    //$or->condition('l.full_name', db_like($string) . '%', 'LIKE');
    $result = $query->condition($or)->range(0, 10)->execute();
    
    foreach ($result as $lead) {
      $matches[$lead->first_name . ' ' . $lead->surname . ' [' . $lead->lid . ']'] = check_plain($lead->first_name . ' ' . $lead->surname); //check_plain(entity_label('kolab_lead', $lead));
    }
  }
  drupal_json_output($matches);
}

/**
 * Task view callback.
 */
function kolab_lead_view($lead) {
  drupal_set_title(entity_label('lead', $lead));
  kolab_lead_tag_new($lead);
  return entity_view('lead', array(entity_id('lead', $lead) => $lead), 'full');
}

/**
 * List all leads
 */
function kolab_lead_list_page() {
	drupal_add_js(drupal_get_path('module', 'kolab_lead') . '/js/kolab_lead_dt.js');

  $action_links = array();
  $action_links[] = l(t('Add new lead'), 'lead/add', array('attributes' => array('class' => array('button-link'))));
  if(module_exists('kolab_lead_import')) {
    $action_links[] = l(t('Import leads'), 'leads/import', array('attributes' => array('class' => array('button-link'))));
  }

  $intro_text = '<p>Filter the listing by clicking on the arrow in the table header. For full lead details and to edit leads click on required table row.</p>';

  $leads = kolab_lead_get_leads();

  $rows[] = array();

  foreach ($leads as $lead_object) {
    $lead = entity_metadata_wrapper('lead', $lead_object);
    $lead_values = $lead->value();

    $statuses = kolab_lead_get_statuses();

    $developments = array();
    
    if(isset($lead->field_developments) && count($lead->field_developments)) {
      foreach ($lead->field_developments as $key => $development_collection) {
        $development_object = $development_collection->field_dev->value();
        //developments
        if($development_object->did) $developments[$development_object->did] = l($development_object->title, 'development/' . $development_object->did);
      }
    }

    $destination = drupal_get_destination();

    $row_classes = array('row');

    $is_new = theme('mark', array('type' => kolab_lead_mark($lead->getIdentifier(), $lead->changed->value())));

    $rows[] = array(
      'data' => array(
        'lid' => array('data' => $lead->getIdentifier(), 'class' => array('lid', 'first')),
        //'first_name' => array('data' => $lead->first_name->value(), 'class' => array('first_name')),
        'first_name' => array('data' => (kolab_lead_access('edit', $lead_object)) ? l($lead->first_name->value(), 'lead/' . $lead->getIdentifier() . '/edit', array('query' => $destination)) . $is_new : $lead->first_name->value() . $is_new, 'class' => array('first_name')),
        //'surname' => array('data' => $lead->surname->value(), 'class' => array('surname')),
        'surname' => array('data' => (kolab_lead_access('edit', $lead_object)) ? l($lead->surname->value(), 'lead/' . $lead->getIdentifier() . '/edit', array('query' => $destination)) : $lead->surname->value(), 'class' => array('surname')),
        'postcode' => array('data' => $lead->postcode->value(), 'class' => array('postcode')),
        'development' => array('data' => (!empty($developments)) ? implode("<br />", $developments) : '-'),
        'status' => array('data' => $statuses[$lead->status->value()], 'class' => array('status', 'status-' . $lead->status->value())),
        //'status' => array('data' => $lead->field_status->value() ? $lead->field_status->value() : '', 'class' => array('status')),
        'created' => array('data' => format_date($lead->created->value(), 'site')),
        //'edit' => array('data' => (kolab_lead_access('edit', $lead_object)) ? l('edit', 'lead/' . $lead->getIdentifier() . '/edit') : '', 'class' => array('edit')),
        'edit' => array('data' => (kolab_lead_access('edit', $lead_object)) ? l('edit', 'lead/' . $lead->getIdentifier() . '/edit', array('query' => $destination)) : '', 'class' => array('edit')),
      ),
      'class' => $row_classes,
    );
  }

  $header = array(
    array('data' => t('ID'), 'class' => array('first lid')),
    array('data' => t('First name'), 'class' => array('first_name')),
    array('data' => t('Surname'), 'class' => array('surname')),
    array('data' => t('Postcode'), 'class' => array('postcode')),
    array('data' => t('Development(s) name'), 'class' => array('development')),
    array('data' => t('Status'), 'class' => array('status')),
    array('data' => t('Date added'), 'class' => array('created')),
    array('data' => '', 'class' => array('edit')),
  );

  $table['header'] = $header;
  $table['attributes'] = array('class' => array('lead-list', 'datatable'));
  $table['caption'] = NULL;
  $table['colgroups'] = array();
  $table['sticky'] = "";
  $table['empty'] = "No leads found.";
  $table['rows'] = $rows;

  return $intro_text . theme('item_list', array('items' => $action_links, 'attributes' => array('class' => array('action-links')))) . theme_table($table);
}

