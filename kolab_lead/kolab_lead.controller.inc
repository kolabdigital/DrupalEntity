<?php

class LeadController extends EntityAPIController {

  public function create(array $values = array()) {
    global $user;
    $values += array(
      'status' => 0,
      'source' => 'manually',
      'source_manually' => '',
      'source_other' => '',
      'title' => '',
      'first_name' => '',
      'surname' => '',
      'investor' => '',
      'mail' => '',
      'phone' => '',
      'phone_mobile' => '',
      'address' => '',
      'address2' => '',
      'county' => '',
      'postcode' => '',
      'message' => '',
      'published' => 1,
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
      //'did' => 0,
    );
    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('lead', $entity);
    $content['author'] = array('#markup' => t('Created by: !author', array('!author' => $wrapper->uid->name->value(array('sanitize' => TRUE)))));
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

/**
 * Lead class.
 */
class Lead extends Entity {
  protected function defaultLabel() {
    return $this->first_name . ' ' . $this->surname;
  }

  protected function defaultUri() {
    return array('path' => 'lead/' . $this->identifier());
  }
}

/**
 * Custom controller for the administrator UI.
 */
class LeadUIController extends EntityDefaultUIController {

  /**
   * Overrides EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['title'] = 'Leads';
    $items[$this->path]['description'] = 'Manage leads';
    $items[$this->path]['access callback'] = 'user_access';
    $items[$this->path]['access arguments'] = array('administer lead entities');
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    unset($items[$this->path . '/add']);
    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::overviewForm().
   */
  public function overviewForm($form, &$form_state) {
    // Pages of local tasks don't use the Menu title
    // @see https://www.drupal.org/node/996302
    drupal_set_title(t('Leads'));

    $form['leads'] = $this->overviewTable();
    $form['pager'] = array('#theme' => 'pager');

    return $form;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTable().
   */
  public function overviewTable($conditions = array()) {
    $header = array(
      array('data' => t('ID'), 'field' => 'lid', 'sort' => 'ASC'),
      array('data' => t('Author'), 'field' => 'l.uid'),
      array('data' => t('Status'), 'field' => 'l.status'),
      array('data' => t('First name'), 'field' => 'l.first_name'),
      array('data' => t('Surname'), 'field' => 'l.surname'),
      array('data' => t('E-mail'), 'field' => 'l.mail'),
      array('data' => t('Created'),'field' => 'l.created'),
      array('data' => t('Source'), 'field' => 'l.source'),
      array('data' => t('Operations')),
    );

    $ids = db_select('lead', 'l')
      ->extend('PagerDefault')->limit(20)
      ->extend('TableSort')->orderByHeader($header)
      ->fields('l', array('lid'))
      ->execute()
      ->fetchCol();

    $leads = kolab_lead_load_multiple($ids);
    $sources_def = kolab_lead_get_sources();

    foreach ($leads as $lead) {
      $row = array();
      $user = user_load($lead->uid);
      $row[] = $lead->lid;
      $row[] = theme('username', array('account' => $user));
      $row[] = (isset($lead->status) && $lead->status == 1) ? t('published') : t('not published');
      $row[] = $lead->first_name;
      $row[] = $lead->surname;
      $row[] = $lead->mail;
      $row[] = format_date($lead->created, 'short');
      $row[] = isset($sources_def[$lead->source]) ? $sources_def[$lead->source] : t('Missing source');
      $operations = array(
        'view' => array('title' => t('View'), 'href' => 'lead/' . $lead->lid . '/view'),
        'edit' => array('title' => t('Edit'), 'href' => 'lead/' . $lead->lid . '/edit', 'query' => drupal_get_destination()),
        'delete' => array('title' => t('Delete'), 'href' => 'lead/' . $lead->lid . '/delete', 'query' => drupal_get_destination()),
      );
      $row[] = array(
        'data' => array(
          '#theme' => 'links__node_operations',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
      $rows[] = $row;
    }

    $render = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No leads available.'),
    );

    return $render;
  }
}


