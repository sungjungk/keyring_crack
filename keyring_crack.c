/*
 *	@file		keyring_crack.c
 *	@brief		TODO brief documentation here.
 *	@history	TODO
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include <string.h>
#include <libsecret/secret.h>
#include <signal.h>
#include <errno.h>


static void
do_crack(SecretService *service) 
{
    GError *error = NULL;
    GList *collections = NULL;
    SecretValue *secret = NULL;
    gchar *label = NULL;
    gboolean ret = false;
    const gchar *pass = NULL;
    gsize len = 0;


    secret_service_load_collections_sync(service, NULL, &error);
    g_assert_no_error (error);

    collections = secret_service_get_collections(service);
    g_assert (collections);

    for (GList *l = collections; l != NULL; l = l->next) {
	    GList* items = secret_collection_get_items(l->data);

	    for (GList *i = items; i != NULL; i = i->next) {
		    /* Get label (target URL) */
		    label = secret_item_get_label(i->data);
		    printf("URL: %-40s", label);
		    secret_value_unref(label);

		    /* Load some secret items */
		    ret = secret_item_load_secret_sync(i->data, NULL, &error);
		    g_assert_no_error (error);
		    g_assert (ret);

		    /* Get secret (target password) */
		    secret = secret_item_get_secret(i->data);
		    g_assert (secret);
		    pass = secret_value_get (secret, &len);
		    printf("\t\tPassword: %s\n", pass);
		    secret_value_unref(secret);
	    }
	    g_list_free(items);
    }
    g_list_free(collections);

    return ;
}

int
main(int argc, char *argv[]) 
{
    GError *error = NULL;
    SecretService *service = NULL;
    
    printf("Let's do crack!!\n\n");
    
    service = secret_service_get_sync (SECRET_SERVICE_OPEN_SESSION, NULL, &error);
    g_assert_no_error (error);
    
    do_crack(service);
    
    secret_service_disconnect();
    
    return EXIT_SUCCESS;
}
/* end of keyring_crack.c */
