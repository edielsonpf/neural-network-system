/**
	@file FileConfigCtrl.c

	@attention Copyright: 

	@attention Project name: <neural-network-train-system>
	@version <version>
	@author Edielson (edielsonpf@gmail.com)
	@date Sep 26, 2011
	@brief <Descrição geral sobre o arquivo>

	<Descrição mais detalhada sobre o propósito do arquivo>

	@since VER	       | DATE	  | COMMENTS
	@since <versao> - <data>  - <Mudanças realizadas nesta versão>

 */

#include "FileConfigCtrl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

/*********************************************************
    Private member functions declaration.
 *********************************************************/
char* FileConfigCtrl_readLine(FILE *file);
Int16 FileConfigCtrl_initializeAtributes();
char* FileConfigCtrl_extractValue(char *line);
Int16 FileConfigCtrl_initializeRequiredDirectory();
Bool8 FileConfigCtrl_checkParameter(char *key, char *value);

/*********************************************************
    Private attributes declaration.
 *********************************************************/
struct tag_FileConfigCtrl
{
	char *pcFileConfig;
};

/*********************************************************
    Private constants.
 *********************************************************/
#define FILE_CONFIG_CTRL_MKDIR "mkdir -p "

/*********************************************************
    Member methods implementation.
 *********************************************************/

/**
 * Class constructor
 *
 * @param char *fileConfig - File to load configurations
 * @return ttag_FileConfigCtrl* - Instance this class
 */
ttag_FileConfigCtrl* FileConfigCtrl_new(char *fileConfig)
{
	ttag_FileConfigCtrl *tagFileConfigCtrl = malloc(sizeof(ttag_FileConfigCtrl));

	if (tagFileConfigCtrl)
	{
		tagFileConfigCtrl->pcFileConfig = strdup(fileConfig);
	}
	else
	{
		syslog(LOG_ERR, "FileConfigCtrl_new: Error initializing FileConfigCtrl_new.");
	}

	return tagFileConfigCtrl;
}

/**
 * Class destructor
 *
 * @param ttag_FileConfigCtrl *this - Instance this class
 * @return TRUE8 if succeeded or FALSE8 if not
 */
Bool8 FileConfigCtrl_delete(ttag_FileConfigCtrl *this) {
	Bool8 bReturn = FALSE8;

	if(this)
	{
		free(this->pcFileConfig);
		free(this);
		bReturn = TRUE8;
	}

	return bReturn;
}

/**
 * Create directory with command mkdir -p
 *
 * @param char *path - path to create directory
 * @return Bool8 - TRUE8 if success or FALSE8 if not
 */
Bool8 FileConfigCtrl_createDirectory(char *path) {
	Bool8 bReturn = TRUE8;
	//                                 mkdir                  path        \0
	Int16 systemCommandSize = strlen(FILE_CONFIG_CTRL_MKDIR) + strlen(path) + 1;
	char *systemCommand = (char*) malloc(systemCommandSize * sizeof(char));
	snprintf(systemCommand, systemCommandSize, "%s%s%c", FILE_CONFIG_CTRL_MKDIR, path, '\0');

	if(system(systemCommand) != 0)
	{
		bReturn = FALSE8;
	}

	free(systemCommand);
	return bReturn;
}

/**
 * Check if key is equals key of value
 *
 * @param char *key - Key to check
 * @param char *value - Value to check
 * @return TRUE8 if succeeded or FALSE8 if not
 */
Bool8 FileConfigCtrl_checkParameter(char *key, char *value)
{
	Int16 index = 0;
	for(; index < strlen(key); index++) // position after '='
	{
		if(key[index] != value[index])
		{
			return FALSE8;
		}
	}

	return TRUE8;
}

/**
 * Get value from key
 *
 * @param ttag_FileConfigCtrl *this - Instance this class
 * @param char *key - Key to find value
 * @return char* - Value key or NULL if error
 */
char* FileConfigCtrl_getValue(ttag_FileConfigCtrl *this, char *key) {
	FILE *pFile = fopen(this->pcFileConfig, "r");
	char *line;
	char *value = NULL;

	if(pFile != NULL)
	{
		while (!feof(pFile))
		{
			line = FileConfigCtrl_readLine(pFile);

			if(line[0] != '#' && FileConfigCtrl_checkParameter(key, line)) // ignore comment
			{
				value = FileConfigCtrl_extractValue(line);
			}
			free(line);
		}
		fclose(pFile);
	}

	return value;
}

/**
 * Read line from file		fclose(pFile);
 *
 * @param FILE *file - File to read line
 * @return char* - line
 */
char* FileConfigCtrl_readLine(FILE *file) {
    int maximumLineLength = 128;
    char *lineBuffer = (char *)malloc(sizeof(char) * maximumLineLength); // malloc initial memory

    if (lineBuffer == NULL)
    {
    	syslog(LOG_ERR, "FileConfigCtrl_readLine: Error allocating memory for line buffer.");
        return NULL;
    }

    char ch = getc(file);
    int count = 0;

    while ((ch != '\n') && (ch != EOF))
    {
        if (count == maximumLineLength)
        {
            maximumLineLength += 128;
            lineBuffer = realloc(lineBuffer, maximumLineLength); // realloc memory
            if (lineBuffer == NULL)
            {
            	syslog(LOG_ERR, "FileConfigCtrl_readLine: Error reallocating space for line buffer.");
                return NULL;
            }
        }
        lineBuffer[count] = ch;
        count++;

        ch = getc(file);
    }
    lineBuffer[count] = '\0';

    return lineBuffer;
}

/**
 * Extract value from the row containing key and value
 *
 * @param char *line - Row from parameter file
 * @return char* - Value key
 */
char* FileConfigCtrl_extractValue(char *line)
{
	Int16 index = 0;
	for(; line[index] != '='; index++); // position after '='
	index++; // '='
	char *value = (char*) malloc((strlen(line) - index + 1) * sizeof(char)); // malloc size parameter

	Int16 indexAux = 0;
	for(; line[index] != '\0'; indexAux++, index++) // set value parameter
	{
		value[indexAux] = line[index];
	}
	value[indexAux] = '\0';

	return value;
}
