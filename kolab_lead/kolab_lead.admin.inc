<?php

/**
 * Just provide some lead info for the entity administration page.
 * This can be expanded to add more information / a list of all created entites, etc.
 */
function kolab_lead_admin_page() {
  $output = 'Welcome to the administration page for your Leads.<br/>';
  $output .= '<a href="/lead/add">Add entity</a>';

  return $output;
}

function kolab_lead_settings_page() {
  $output = drupal_get_form('kolab_lead_settings_form');
  return drupal_render($output);
}

function kolab_lead_settings_form($form, &$form_state) {

  $form['settings'] = array(
    '#type' => 'item',
    '#title' => t('Settings'),
  );

  return $form;
}

/**
 * Add new lead page callback.
 */
function kolab_lead_add() {
  $lead = entity_create('lead', array('source' => 'manually'));
  drupal_set_title(t('Add new lead'));
  $output = '<div class="intro-markup"><span class="form-required">*</span> Mandatory field</div>';
  $kolab_lead_form = drupal_get_form('kolab_lead_form', $lead);
  $output .= drupal_render($kolab_lead_form);
  return $output;
}

/**
 * Edit lead page callback.
 */
function kolab_lead_edit($lead) {
  drupal_set_title(entity_label('lead', $lead) . ' (ID: '. $lead->lid .')');
  kolab_lead_tag_new($lead);
  $output = '<div class="intro-markup"><span class="form-required">*</span> Mandatory field</div>';
  $lead_form = drupal_get_form('kolab_lead_form', $lead);
  $output .= drupal_render($lead_form);
  return $output;
}

/**
 * Lead Form.
 */
function kolab_lead_form($form, &$form_state, $lead) {
  $form_state['lead'] = $lead;

  $source_types = kolab_lead_get_sources();
  $investor_statuses = kolab_lead_get_investor_statuses();
  $statuses = kolab_lead_get_statuses();
  $titles = kolab_lead_get_titles();

  $form['top'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('top-box')),
    '#weight' => -5,
  );

  $form['top']['status'] = array(
    '#type' => 'select',
    '#title' => t('Lead status'),
    '#options' => $statuses,
    '#default_value' => $lead->status,
    //'#suffix' => '<div class="form-item lead-status-agenda"><a href="#">Key</a><div class="agenda">'. theme('kolab_lead_status_agenda', array()) .'</div></div>',
    '#required' => TRUE,
  );

  $form['top']['investor'] = array(
    '#type' => 'select',
    '#title' => t('Investor status'),
    '#options' => $investor_statuses,
    '#default_value' => $lead->investor,
    '#required' => FALSE,
  );

  $form['top']['source'] = array(
    '#type' => 'select',
    '#title' => t('Lead source'),
    '#options' => $source_types,
    '#default_value' => $lead->source,
    '#required' => TRUE,
  );

  $form['general'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('general-box')),
    '#weight' => -4,
  );

  /* first column */
  $form['general']['first'] = array(
    '#prefix' => '<div class="first-col form-col">',
    '#suffix' => '</div>',
  );

  $form['general']['first']['title'] = array(
    '#type' => 'select',
    '#title' => t('Title'),
    '#options' => $titles,
    '#default_value' => $lead->title,
    '#required' => FALSE,
  );

  $form['general']['first']['first_name'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('First name'),
    '#default_value' => $lead->first_name,
    '#disabled' => (isset($lead->uid_ref) && $lead->uid_ref) ? TRUE : FALSE,
  );

  $form['general']['first']['surname'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Surname'),
    '#default_value' => $lead->surname,
    '#disabled' => (isset($lead->uid_ref) && $lead->uid_ref) ? TRUE : FALSE,
  );

  /* second column */
  $form['general']['second'] = array(
    '#prefix' => '<div class="second-col form-col">',
    '#suffix' => '</div>',
  );

  $form['general']['second']['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#maxlength' => EMAIL_MAX_LENGTH,
    '#required' => TRUE,
    '#default_value' => $lead->mail,
    '#disabled' => (isset($lead->uid_ref) && $lead->uid_ref) ? TRUE : FALSE,
  );

  $form['general']['second']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Home phone'),
    '#maxlength' => 20,
    '#required' => TRUE,
    '#default_value' => $lead->phone,
  );

  $form['general']['second']['phone_mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile phone'),
    '#maxlength' => 20,
    '#required' => FALSE,
    '#default_value' => $lead->phone_mobile,
  );

  /* third column */
  $form['general']['third'] = array(
    '#prefix' => '<div class="third-col form-col">',
    '#suffix' => '</div>',
  );

  $form['general']['third']['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address line 1'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => $lead->address,
  );

  $form['general']['third']['address2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address line 2'),
    '#maxlength' => 255,
    '#required' => FALSE,
    '#default_value' => $lead->address2,
  );

  $form['general']['third']['county'] = array(
    '#type' => 'textfield',
    '#title' => t('County'),
    '#maxlength' => 255,
    '#required' => FALSE,
    '#default_value' => $lead->county,
  );

  $form['general']['third']['postcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Postcode'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => $lead->postcode,
  );

  // $form['message'] = array(
  //   '#type' => 'textarea',
  //   '#title' => t('Lead message'),
  //   '#default_value' => $lead->message,
  // );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $lead->uid,
  );

  // $form['did'] = array(
  //   '#type' => 'select',
  //   '#title' => t('Development'),
  //   '#options' => $developments,
  //   '#default_value' => $lead->did,
  //   '#required' => FALSE,
  // );

  field_attach_form('lead', $lead, $form, $form_state);

  // Attach field widgets.
  // $langcode = entity_language('user', $account);
  // field_attach_form('user', $account, $form['left']['details'], $form_state, $langcode);

  // if (!empty($lead->lid) && user_access('administer lead entities') && isset($lead->status)) {
  //   $form['status'] = array(
  //     '#type' => 'checkbox',
  //     '#title' => t('Enabled'),
  //     '#default_value' => $lead->status,
  //   );
  // }

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
    '#type' => 'actions'
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
    '#submit' => $submit + array('kolab_lead_form_submit'),
  );

  // Show Delete button if we edit lead.
  $lead_id = entity_id('lead', $lead);
  if (!empty($lead_id) && kolab_lead_access('delete', $lead) && !$lead->uid_ref) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete lead'),
      '#submit' => array('kolab_lead_form_submit_delete'),
    );
  }

  // Show profile button if lead has account.
  if (isset($lead->uid_ref) && $lead->uid_ref) {
    $form['actions']['profile'] = array(
      '#type' => 'submit',
      '#value' => t('View user profile'),
      '#submit' => array('kolab_lead_form_submit_account'),
    );
  }

  $form['#validate'][] = 'kolab_lead_form_validate';

  drupal_add_js(drupal_get_path('module', 'kolab_lead') . '/js/kolab_lead.js');

  return $form;
}

/**
 * Lead validate handler.
 */
function kolab_lead_form_validate($form, &$form_state) {
  //validate lead
  $values = $form_state['values'];
  $lead = $form_state['lead'];

  //check email format
  if(!kolab_lead_validate('mail', array($values['mail']))) {
    form_set_error('mail', 'Email address is not valid.');
  }

  //check email is unique
  $lid = (isset($lead->lid) && $lead->lid) ? $lead->lid : 0;
  if(!kolab_lead_validate('mail_unique', array($values['mail'], $lid))) {
    form_set_error('mail', 'Email address already exists.');
  }

  //check status
  if(!kolab_lead_validate('status', array($values['status']))) {
    form_set_error('status', 'Status doesn\'t exist.');
  }

  //check source
  if(!kolab_lead_validate('source', array($values['source']))) {
    form_set_error('source', 'Source doesn\'t exist.');
  }

  //check title
  if(!kolab_lead_validate('title', array($values['title']))) {
    form_set_error('title', 'Title is not valid.');
  }

  //check first name
  if(!kolab_lead_validate('first_name', array($values['first_name']))) {
    form_set_error('first_name', 'First name is not valid.');
  }

  //check surname
  if(!kolab_lead_validate('surname', array($values['surname']))) {
    form_set_error('surname', 'Surname is not valid.');
  }

  //check investor
  if(!kolab_lead_validate('investor', array($values['investor']))) {
    form_set_error('investor', 'Investor value is not valid.');
  }

  //check phone
  if(!kolab_lead_validate('phone', array($values['phone']))) {
    form_set_error('phone', 'Phone number is not valid.');
  }

  if(!kolab_lead_validate('phone', array($values['phone_mobile']))) {
    form_set_error('phone_mobile', 'Mobile phone number is not valid.');
  }

  //check address
  if(!kolab_lead_validate('address', array($values['address']))) {
    form_set_error('address', 'Address line 1 is not valid.');
  }

  if(!kolab_lead_validate('address', array($values['address2']))) {
    form_set_error('address2', 'Address line 2 is not valid.');
  }

  //check county
  if(!kolab_lead_validate('county', array($values['county']))) {
    form_set_error('county', 'County is not valid.');
  }

  //check postcode
  if(!kolab_lead_validate('postcode', array($values['postcode']))) {
    form_set_error('postcode', 'Postcode is not valid.');
  }

  //check development ID
  // if(!kolab_lead_validate('development_exists', $values['field_development'][LANGUAGE_NONE][0]['target_id'])) {
  //   form_set_error('third][field_development', 'Development with ID doesn\'t exist.');
  // }
}

/**
 * Lead submit handler.
 */
function kolab_lead_form_submit($form, &$form_state) {
  $lead = $form_state['lead'];

  entity_form_submit_build_entity('lead', $lead, $form, $form_state);

  kolab_lead_save($lead);

  $lead_uri = entity_uri('lead', $lead);

  $form_state['redirect'] = 'leads';

  drupal_set_message(t('%title saved.', array('%title' => entity_label('lead', $lead))));
}

/**
 * View profile account
 */
function kolab_lead_form_submit_account($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  
  $lead = $form_state['lead']; 
  $form_state['redirect'] = 'user/' . $lead->uid_ref;
}

/**
 * Lead delete.
 */
function kolab_lead_form_submit_delete($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $lead = $form_state['lead'];
  $lead_uri = entity_uri('lead', $lead);
  $form_state['redirect'] = array($lead_uri['path'] . '/delete', array('query' => $destination));
}

/**
 * Delete confirmation form.
 */
function kolab_lead_delete_form($form, &$form_state, $lead) {
  $form_state['lead'] = $lead;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['lead_type_id'] = array('#type' => 'value', '#value' => entity_id('lead' ,$lead));
  $lead_uri = entity_uri('lead', $lead);
  return confirm_form($form,
    t('Are you sure you want to delete the lead: %title?', array('%title' => entity_label('lead', $lead))),
    $lead_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function kolab_lead_delete_form_submit($form, &$form_state) {
  $lead = $form_state['lead'];
  kolab_lead_delete($lead->lid);
  drupal_set_message(t('Lead %title deleted.', array('%title' => entity_label('lead', $lead))));
  $form_state['redirect'] = '<front>';
}

/**
 * Page callback: Form constructor for the leads administration form.
 */
function kolab_lead_admin_content($form, $form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return kolab_lead_multiple_delete_confirm($form, $form_state, array_filter($form_state['values']['leads']));
  }
  $form['filter'] = kolab_lead_filter_form();
  $form['#submit'][] = 'kolab_lead_filter_form_submit';
  $form['admin'] = kolab_lead_admin_list();

  return $form;
}

/**
 * Form builder: Builds the lead administration overview.
 */
function kolab_lead_admin_list() {
  $admin_access = user_access('administer lead entities');

  // Build the 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
    '#access' => $admin_access,
  );
  $options = array();
  $operations = kolab_lead_operations();

  foreach ($operations as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'approve',
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('kolab_lead_admin_list_validate'),
    '#submit' => array('kolab_lead_admin_list_submit'),
  );

  // Build the sortable table header.
  $header = array(
    'id' => array('data' => t('ID'), 'field' => 'l.lid'),
    'author' => t('Author'),
    'source' => array('data' => t('Source'), 'field' => 'l.source'),
    'published' => array('data' => t('Published'), 'field' => 'l.published'),
    'full_name' => t('Full name'),
    //'plot' => array('data' => t('Plots')),
    'mail' => array('data' => t('E-mail'), 'field' => 'l.mail'),
    'created' => array('data' => t('Created'), 'field' => 'l.created'),
    'changed' => array('data' => t('Updated'), 'field' => 'l.changed', 'sort' => 'desc'),
    'operations' => array('data' => t('Operations'))
  );

  $query = db_select('lead', 'l')->extend('PagerDefault')->extend('TableSort');
  $query->addTag('lead_admin_filter');
  kolab_lead_build_filter_query($query);

  if (user_access('view own leads') && $own_unpublished = db_query('SELECT lid FROM {lead} WHERE uid = :uid AND published = :published', array(':uid' => $GLOBALS['user']->uid, ':published' => 0))->fetchCol()) {
    $query->condition(db_or()->condition('l.published', 1)->condition('l.lid', $own_unpublished, 'IN'));
  }
  else {
    // If not, restrict the query to published nodes.
    $query->condition('l.published', 1);
  }

  $lids = $query
    ->fields('l',array('lid'))
    ->limit(50)
    ->orderByHeader($header)
    ->addTag('kolab_lead_access')
    ->execute()
    ->fetchCol();
  $leads = kolab_lead_load_multiple($lids);

  // Prepare the list of leads.
  $destination = drupal_get_destination();
  $options = array();
  $sources_def = kolab_lead_get_sources();

  foreach ($leads as $lead) {

    $author_name = format_username(user_load($lead->uid));
    if (drupal_strlen($author_name) > 20) $author_name = drupal_substr($author_name, 0, 15) . '...';

    // $plots = kolab_buyer_get_plots($lead->lid);
    // $plot_names = array();
    // if($plots) {
    //   foreach ($plots as $plot) {
    //     $plot_uri = entity_uri('plot', $plot);
    //     $plot_names[] = l(entity_label('plot', $plot), $plot_uri['path']);
    //   }
    // }

    $options[$lead->lid] = array(
      'id' => $lead->lid,
      'author' => check_plain($author_name),
      'source' => isset($sources_def[$lead->source]) ? $sources_def[$lead->source] : t('Missing source'),
      'published' => $lead->published ? t('yes') : t('no'),
      //'full_name' => $lead->first_name . ' ' . $lead->surname,
      'full_name' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $lead->first_name . ' ' . $lead->surname,
          '#href' => 'lead/' . $lead->lid,
          '#suffix' => ' ' . theme('mark', array('type' => kolab_lead_mark($lead->lid, $lead->changed))),
        ),
      ),
      // 'plot' => (empty($plot_names)) ? '-' : implode(", <br />", $plot_names),
      'mail' => $lead->mail,
      'created' => format_date($lead->created, 'short'),
      'changed' => format_date($lead->changed, 'short'),
    );
    
    // Build a list of all the accessible operations for the current lead.
    $operations = array();
    // if (kolab_lead_access('view', $lead)) {
    //   $operations['view'] = array(
    //     'title' => t('view'),
    //     'href' => 'lead/' . $lead->lid,
    //   );
    // }
    if (kolab_lead_access('edit', $lead)) {
      $operations['edit'] = array(
        'title' => t('edit'),
        'href' => 'lead/' . $lead->lid . '/edit',
        'query' => $destination,
      );
    }
    if (kolab_lead_access('delete', $lead)) {
      $operations['delete'] = array(
        'title' => t('delete'),
        'href' => 'lead/' . $lead->lid . '/delete',
        'query' => $destination,
      );
    }
    $options[$lead->lid]['operations'] = array();
    if (count($operations) > 1) {
      // Render an unordered list of operations links.
      $options[$lead->lid]['operations'] = array(
        'data' => array(
          '#theme' => 'links__lead_operations',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
    elseif (!empty($operations)) {
      // Render the first and only operation as a link.
      $link = reset($operations);
      $options[$lead->lid]['operations'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link['title'],
          '#href' => $link['href'],
          '#options' => array('query' => $link['query']),
        ),
      );
    }
  }

  // Only use a tableselect when the current user is able to perform any
  // operations.
  if ($admin_access) {
    $form['leads'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No leads available.'),
    );
  }
  // Otherwise, use a simple table.
  else {
    $form['leads'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
      '#empty' => t('No leads available.'),
    );
  }

  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

/**
 * Validate kolab_lead_admin_list form submissions.
 *
 * Checks whether any leads have been selected to perform the chosen 'Update
 * option' on.
 */
function kolab_lead_admin_list_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['leads']) || !count(array_filter($form_state['values']['leads']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Process kolab_lead_admin_list form submissions.
 * Executes the chosen 'Update option' on the selected leads.
 */
function kolab_lead_admin_list_submit($form, &$form_state) {
  $operations = kolab_lead_operations();
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked leads
  $leads = array_filter($form_state['values']['leads']);
  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($leads), $operation['callback arguments']);
    }
    else {
      $args = array($leads);
    }
    call_user_func_array($function, $args);

    cache_clear_all();
  }
  else {
    // We need to rebuild the form to go to a second step. For example, to
    // show the confirmation form for the deletion of leads.
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Returns the lead administration filters form array to kolab_lead_admin_list().
 */
function kolab_lead_filter_form() {
  $session = isset($_SESSION['kolab_lead_overview_filter']) ? $_SESSION['kolab_lead_overview_filter'] : array();
  $filters = kolab_lead_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__lead',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    
    $value = $filters[$type]['options'][$value];
  
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
    if (in_array($type, array('type', 'language'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }

  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#default_value' => '[any]',
    );
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * List lead administration filters that can be applied.
 *
 * @return
 *   An associative array of filters.
 */
function kolab_lead_filters() {
  // Regular filters
  $filters['published'] = array(
    'title' => t('Status'),
    'options' => array(
      '[any]' => t('any'),
      'published-1' => t('published'),
      'published-0' => t('not published'),
    ),
  );

  $filters['source'] = array(
    'title' => t('Source'),
    'options' => array(
      '[any]' => t('any'),
    ) + kolab_lead_get_sources(),
  );

  return $filters;
}

/**
 * Applies filters for lead administration filters based on session.
 *
 * @param $query
 *   A SelectQuery to which the filters should be applied.
 */
function kolab_lead_build_filter_query(SelectQueryInterface $query) {
  // Build query
  $filter_data = isset($_SESSION['kolab_lead_overview_filter']) ? $_SESSION['kolab_lead_overview_filter'] : array();
  foreach ($filter_data as $index => $filter) {
    list($key, $value) = $filter;
    switch ($key) {
      case 'published':
        // Note: no exploitable hole as $key/$value have already been checked when submitted
        list($key, $value) = explode('-', $value, 2);
      case 'source':
        $query->condition('l.' . $key, $value);
        break;
    }
  }
}

/**
 * Form submission handler for kolab_lead_filter_form().
 */
function kolab_lead_filter_form_submit($form, &$form_state) {
  $filters = kolab_lead_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          // Flatten the options array to accommodate hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($flat_options[$form_state['values'][$filter]])) {
            $_SESSION['kolab_lead_overview_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['kolab_lead_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['kolab_lead_overview_filter'] = array();
      break;
  }
}

/**
 * Make mass update of leads, changing all leads in the $leads array
 * to update them with the field values in $updates.
 *
 * IMPORTANT NOTE: This function is intended to work when called from a form
 * submission handler. Calling it outside of the form submission process may not
 * work correctly.
 *
 * @param array $leads
 *   Array of lead lids to update.
 * @param array $updates
 *   Array of key/value pairs with lead field names and the value to update that
 *   field to.
 */
function kolab_lead_mass_update($leads, $updates) {
  // We use batch processing to prevent timeout when updating a large number
  // of leads.
  if (count($leads) > 10) {
    $batch = array(
      'operations' => array(
        array('_kolab_lead_mass_update_batch_process', array($leads, $updates))
      ),
      'finished' => '_kolab_lead_mass_update_batch_finished',
      'title' => t('Processing'),
      // We use a single multi-pass operation, so the default
      // 'Remaining x of y operations' message will be confusing here.
      'progress_message' => '',
      'error_message' => t('The update has encountered an error.'),
      // The operations do not live in the .module file, so we need to
      // tell the batch engine which file to load before calling them.
      'file' => drupal_get_path('module', 'kolab_lead') . '/kolab_lead.admin.inc',
    );
    batch_set($batch);
  }
  else {
    foreach ($leads as $lid) {
      _kolab_lead_mass_update_helper($lid, $updates);
    }
    drupal_set_message(t('The update has been performed.'));
  }
}

/**
 * Executes a batch operation for kolab_lead_mass_update().
 *
 * @param array $leads
 *   An array of lead IDs.
 * @param array $updates
 *   Associative array of updates.
 * @param array $context
 *   An array of contextual key/values.
 */
function _kolab_lead_mass_update_batch_process($leads, $updates, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($leads);
    $context['sandbox']['leads'] = $leads;
  }

  // Process leads by groups of 5.
  $count = min(5, count($context['sandbox']['leads']));
  for ($i = 1; $i <= $count; $i++) {
    // For each nid, load the lead, reset the values, and save it.
    $lid = array_shift($context['sandbox']['leads']);
    $lead = _kolab_lead_mass_update_helper($lid, $updates);

    // Store result for post-processing in the finished callback.
    $context['results'][] = l($lead->first_name . ' ' . $lead->surname . '\'s lead', 'lead/' . $lead->lid);

    // Update our progress information.
    $context['sandbox']['progress']++;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Menu callback: Reports the status of batch operation for kolab_lead_mass_update().
 *
 * @param bool $success
 *   A boolean indicating whether the batch mass update operation successfully
 *   concluded.
 * @param int $results
 *   The number of leads updated via the batch mode process.
 * @param array $operations
 *   An array of function calls (not used in this function).
 */
function _kolab_lead_mass_update_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The update has been performed.'));
  }
  else {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
    $message = format_plural(count($results), '1 item successfully processed:', '@count items successfully processed:');
    $message .= theme('item_list', array('items' => $results));
    drupal_set_message($message);
  }
}

/**
 * Updates individual leads when fewer than 10 are queued.
 *
 * @param $lid
 *   ID of lead to update.
 * @param $updates
 *   Associative array of updates.
 *
 * @return object
 *   An updated lead object.
 *
 * @see kolab_lead_mass_update()
 */
function _kolab_lead_mass_update_helper($lid, $updates) {
  $lead = kolab_lead_load($lid);
  // For efficiency manually save the original lead before applying any changes.
  $lead->original = clone $lead;
  foreach ($updates as $name => $value) {
    $lead->$name = $value;
  }
  kolab_lead_save($lead);
  return $lead;
}


/**
 * Multiple lead deletion confirmation form for kolab_lead_admin_content().
 */
function kolab_lead_multiple_delete_confirm($form, &$form_state, $leads) {
  $form['leads'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter returns only elements with TRUE values
  foreach ($leads as $lid => $value) {
    $result = db_query('SELECT first_name, surname FROM {lead} WHERE lid = :lid', array(':lid' => $lid));
    $record = $result->fetchObject();
    $title = $record->first_name . ' ' . $record->surname . '\'s lead';
    $form['leads'][$lid] = array(
      '#type' => 'hidden',
      '#value' => $lid,
      '#prefix' => '<li>',
      '#suffix' => check_plain($title) . "</li>\n",
    );
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'kolab_lead_multiple_delete_confirm_submit';
  $confirm_question = format_plural(count($leads), 'Are you sure you want to delete this item?', 'Are you sure you want to delete these items?');
  return confirm_form($form, $confirm_question, 'admin/content/lead', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for kolab_lead_multiple_delete_confirm().
 */
function kolab_lead_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    kolab_lead_delete_multiple(array_keys($form_state['values']['leads']));
    cache_clear_all();
    $count = count($form_state['values']['leads']);
    watchdog('kolab_lead', 'Deleted @count leads.', array('@count' => $count));
    drupal_set_message(format_plural($count, 'Deleted 1 lead.', 'Deleted @count leads.'));
  }
  $form_state['redirect'] = 'admin/content/lead';
}
