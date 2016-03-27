#include "e.h"
#include "e_mod_main.h"

/***************************************************************************/
/**/
/* actual module specifics */

static E_Module *layout_module = NULL;
static E_Client_Hook *hook = NULL;

int sort_cb(const void *d1, const void *d2);
int border_type_to_layer(int type);

static void _e_module_layout_cb_hook(void *data, E_Client *bd)
{
  Eina_List *clientList = NULL;
  Eina_List *list = NULL;
  Eina_List *l = NULL;
  int i = 0;

  /* FIXME: make some modification based on policy */
  printf("Window:\n"
         "  Title:      [%s][%s]\n"
         "  Class:      %s::%s\n"
         "  Geometry:   %ix%i+%i+%i\n"
         "  New:        %i\n"
         "  NetWM Type: %i\n"
         , bd->icccm.title, bd->netwm.name
         , bd->icccm.name, bd->icccm.class
         , bd->x, bd->y, bd->w, bd->h
         , bd->new_client
         , bd->netwm.type);

  printf("open centered\n");
  bd->e.state.centered = 1;
  bd->icccm.base_w = 1;
  bd->icccm.base_h = 1;
  bd->icccm.min_w = 1;
  bd->icccm.min_h = 1;
  bd->icccm.max_w = bd->w;
  bd->icccm.max_h = bd->h;

  clientList = e_client_focus_stack_get();

  printf("num: %d\n", eina_list_count(clientList));
  printf("Current window type: %d\n", bd->netwm.type);

  for (l = e_client_focus_stack_get(); l; l = l->next)
  {
    E_Client *border = l->data;

    printf("before: window type is: %d\n", border->netwm.type);
  }

  printf("pre sort\n");
  clientList = e_client_focus_stack_get();

  if (!clientList) printf("!clientList\n");

  printf("num2: %d\n", eina_list_count(clientList));
  list = eina_list_sort(clientList, eina_list_count(clientList), sort_cb);
  /*if (eina_list_alloc_error())
  {
    fprintf(stderr, "ERROR: Memory is low. List Sorting failed.\n");
    exit(-1);
  }*/

  for (l = e_client_focus_stack_get(); l; l = l->next)
  {
    E_Client *border = l->data;

    printf("after: window type is: %d\n", border->netwm.type);
  }

  if ((bd->icccm.transient_for != 0) ||
      (bd->netwm.type == ECORE_X_WINDOW_TYPE_NORMAL))
  {

  }
  else if ((bd->icccm.transient_for != 0) ||
           (bd->netwm.type == ECORE_X_WINDOW_TYPE_UTILITY))
  {
    e_client_unmaximize(bd, E_MAXIMIZE_BOTH);
    //e_client_move(bd, 0, 0);
  }
  /*else
  {
    printf ("open maxmized\n");
    e_border_unmaximize(bd, E_MAXIMIZE_BOTH);
    e_border_move(bd,
                  bd->zone->x + (bd->zone->w / 2),
                  bd->zone->y + (bd->zone->h / 2));
    e_border_resize(bd, 1, 1);
    if (bd->bordername) evas_stringshare_del(bd->bordername);
    bd->bordername = evas_stringshare_add("borderless");
    bd->client.icccm.base_w = 1;
    bd->client.icccm.base_h = 1;
    bd->client.icccm.min_w = 1;
    bd->client.icccm.min_h = 1;
    bd->client.icccm.max_w = 32767;
    bd->client.icccm.max_h = 32767;
    bd->client.icccm.min_aspect = 0.0;
    bd->client.icccm.max_aspect = 0.0;
  }*/
  else
  {
    e_client_maximize(bd, E_MAXIMIZE_FILL | E_MAXIMIZE_BOTH);
  }
}

int sort_cb(const void *d1, const void *d2)
{
  const E_Client *border1 = d1;
  const E_Client *border2 = d2;
  int layer1 = 0;
  int layer2 = 0;

  printf("sort\n");

  if (!border1) return(1);
  if (!border2) return(-1);

  layer1 = border_type_to_layer(border1->netwm.type);
  layer2 = border_type_to_layer(border2->netwm.type);
  printf("layer 1: %d, layer 2: %d\n", layer1, layer2);

  if (layer1 < layer2)
  {
    return -1;
  }
  else if (layer2 < layer1)
  {
    return 1;
  }

  return 0;
}


int border_type_to_layer(int type)
{
  int layer = 0;

  switch (type)
  {
  case ECORE_X_WINDOW_TYPE_DESKTOP:
    layer = 0;
    break;

  case ECORE_X_WINDOW_TYPE_MENU:
    layer = 1;
    break;

  case ECORE_X_WINDOW_TYPE_UTILITY:
    layer = 2;
    break;

  case ECORE_X_WINDOW_TYPE_UNKNOWN:
    // fall through...

  default:
    layer = 2;
  }

  return layer;
}


/*
ECORE_X_WINDOW_TYPE_DESKTOP  ==> OISPMapViewer
  Layer 0

ECORE_X_WINDOW_TYPE_DOCK
ECORE_X_WINDOW_TYPE_TOOLBAR
ECORE_X_WINDOW_TYPE_MENU  ==> OISPInterface
  Layer 1

ECORE_X_WINDOW_TYPE_UTILITY  ==> OISPControl_keyb
  Layer 2
  Invisible behind Layer 0 or minimized.
  Needs to have keyboard input focus

ECORE_X_WINDOW_TYPE_SPLASH
ECORE_X_WINDOW_TYPE_DIALOG

ECORE_X_WINDOW_TYPE_NORMAL   => normal debug windows


ECORE_X_WINDOW_TYPE_UNKNOWN

 All other window classes should popup in the front

 */

/**/
/***************************************************************************/

/***************************************************************************/
/**/

/**/
/***************************************************************************/

/***************************************************************************/
/**/
/* module setup */
EAPI E_Module_Api e_modapi =
{
  E_MODULE_API_VERSION,
  "OILM"
};

EAPI void *
e_modapi_init(E_Module *m)
{
  char buf[4096];
  layout_module = m;

  hook = e_client_hook_add(E_CLIENT_HOOK_EVAL_POST_FETCH,
                           _e_module_layout_cb_hook, NULL);

  /*snprintf(buf, sizeof(buf), "%s/e-module-battery.edj", e_module_dir_get(m));
  e_configure_registry_category_add("advanced", 80, _("Advanced"), NULL,
                                    "preferences-advanced");
  e_configure_registry_item_add("advanced/oilm", 100, _("Open Infotainment Layer Manager"),
                                NULL, buf, e_int_config_battery_module);*/

  return m;
}

EAPI int
e_modapi_shutdown(E_Module *m)
{
  if (hook)
  {
    e_client_hook_del(hook);
    hook = NULL;
  }
  layout_module = NULL;
  return 1;
}

EAPI int
e_modapi_save(E_Module *m)
{
  return 1;
}
