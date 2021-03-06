
/***************************************************************************
 *                                 
 * @ Danstan Otieno Onyango
 * Impalapay Kenya
 *
 ***************************************************************************/
/* <DESC>
 * simple HTTP POST using the easy interface
 * </DESC>
 */
#include <stdio.h>
#include "curl/curl.h"
#include "ctosapi.h"
#include "cJSON.h"
#include <stdlib.h>





int validation=0;

char *jsonout;

/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
	char *jsonout;cJSON *json;
	
	json=cJSON_Parse(text);
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		jsonout=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",jsonout);
		free(jsonout);
	}
}

/* Read a file, parse, render back, etc. */
void dofile(char *filename)
{
	FILE *f;long len;char *data;
	
	f=fopen(filename,"rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);
	doit(data);
	free(data);
}

/* Used by some code below as an example datatype. */
struct record {const char *precision;double lat,lon;const char *address,*city,*state,*zip,*country; };


int curlpostmain(BYTE pin[4],BYTE amount[5],  BYTE *msg) {
    
    BYTE key;
    BYTE sBuf[128];
    
    {
        cJSON *root,*fmt,*img,*thm,*fld;int i;	/* declare a few. */
        
	
	
	//build json object-string
	root=cJSON_CreateObject();
        cJSON_AddItemToObject(root, "username", cJSON_CreateString("familypos"));
        cJSON_AddItemToObject(root, "password", cJSON_CreateString("password"));
        cJSON_AddItemToObject(root, "merchantCard", cJSON_CreateString("4592530000000218"));
        cJSON_AddItemToObject(root, "merchantCVV", cJSON_CreateString("022"));
        cJSON_AddItemToObject(root, "recipientCard", cJSON_CreateString("32556462346"));
        cJSON_AddItemToObject(root, "recipientCVV", cJSON_CreateString("022"));
	cJSON_AddItemToObject(root, "amount",cJSON_CreateString(amount));
	cJSON_AddItemToObject(root, "accountnumber",cJSON_CreateString("011535272848"));
	cJSON_AddItemToObject(root, "pin", cJSON_CreateString(pin));
        cJSON_AddItemToObject(root, "card_number", cJSON_CreateString("4592530000000218"));
        cJSON_AddItemToObject(root, "recipientcard_type", cJSON_CreateString(msg));
	cJSON_AddItemToObject(root, "merchantPin",cJSON_CreateString("24523"));
        cJSON_AddItemToObject(root, "recipientCardexpiry_date",cJSON_CreateString("24523"));
        cJSON_AddItemToObject(root, "auth_code",cJSON_CreateString("24523"));
	
	jsonout=cJSON_Print(root);	cJSON_Delete(root);	/*printf("%s\n",jsonout);	free(jsonout);	/* Print to text, Delete the cJSON, print it, release the string. */

	
        CURL *curl;
        CURLcode res;
        ClearScreen(4, 14);
        CTOS_LCDTPrintXY(3, 5, "Sending........");

        curl = curl_easy_init();
        if (curl) {
            //char* jsonObj = "{ \"pin\" : \"4444\" , \"amount\" :\"10000\", \"card_number\" : \"72828276766262\" }"; 
            curl_easy_setopt(curl, CURLOPT_URL, "http://196.216.73.150:9990/familypos/request/bankWitdrawal");
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "charsets: utf-8");
            /* example.com is redirected, so we tell libcurl to follow redirection */
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            /* Perform the request, res will get the return code */
             curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout);
            res = curl_easy_perform(curl);
            //CTOS_LCDTPrintXY(4, 4, " Response is");
            /* Check for errors */
            long http_code = 0;
            curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK)
                 {
                ClearScreen(4, 14);
                CTOS_LCDTPrintXY(3, 5, " Successful");
                CTOS_KBDGet(&key);
                CTOS_Delay(3000);
                validation=1;
                curl_easy_cleanup(curl);
                return 1;
            }
            else if(http_code == 401)
            {
                ClearScreen(4, 14);
                CTOS_LCDTPrintXY(3, 5, " Wrong PIN");
                CTOS_KBDGet(&key);
                CTOS_Delay(3000);
                 validation=0;
                 curl_easy_cleanup(curl);
                return 0;
            }
            ClearScreen(4, 14);
            CTOS_LCDTPrintXY(3, 5, " Failed");
                CTOS_KBDGet(&key);
                return 0;
            curl_easy_cleanup(curl);
        }
        
    }


}