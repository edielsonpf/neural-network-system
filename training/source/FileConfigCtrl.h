/**
	@file FileConfigCtrl.h

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

#ifndef FILECONFIGCTRL_H_
#define FILECONFIGCTRL_H_

/*********************************************************
    Includes
*********************************************************/
#include "GenericTypeDefs.h"

/*********************************************************
    Defines
*********************************************************/
#define DEFAULT_TRAIN_CONFIG_FILE			"train_config.cfg"

#define PARAMETER_NEURAL_NETWORK_INPUT				"NEURAL_NETWORK_INPUT"
#define PARAMETER_NEURAL_NETWORK_OUTPUT				"NEURAL_NETWORK_OUTPUT"
#define PARAMETER_NEURAL_NETWORK_LAYERS				"NEURAL_NETWORK_LAYERS"
#define PARAMETER_NEURAL_NETWORK_HIDDEN_NEURONS		"NEURAL_NETWORK_HIDDEN_NEURONS"
#define PARAMETER_NEURAL_NETWORK_ERROR				"NEURAL_NETWORK_ERROR"
#define PARAMETER_NEURAL_NETWORK_MAX_EPOCHS			"NEURAL_NETWORK_MAX_EPOCHS"
#define PARAMETER_NEURAL_NETWORK_REPORTS_EPOCHS		"NEURAL_NETWORK_REPORTS_EPOCHS"
#define PARAMETER_NEURAL_NETWORK_INPUT_FILE			"NEURAL_NETWORK_TRAIN_INPUT_FILE"
#define PARAMETER_NEURAL_NETWORK_OUTPUT_FILE		"NEURAL_NETWORK_TRAIN_OUTPUT_FILE"
#define PARAMETER_NEURAL_NETWORK_LEARNING_RATE		"NEURAL_NETWORK_LEARNING_RATE"
#define PARAMETER_NEURAL_NETWORK_DECREASE_LR		"NEURAL_NETWORK_DECREASE_LR"
#define PARAMETER_NEURAL_NETWORK_METHOD 			"NEURAL_NETWORK_TRAIN_METHOD"

/*********************************************************
    Public types.
*********************************************************/
typedef struct tag_FileConfigCtrl ttag_FileConfigCtrl;

/*********************************************************
    Public member function definition.
*********************************************************/
Bool8 FileConfigCtrl_createDirectory(char *path);
Bool8 FileConfigCtrl_delete(ttag_FileConfigCtrl *this);
ttag_FileConfigCtrl* FileConfigCtrl_new(char *fileConfig);
char* FileConfigCtrl_getValue(ttag_FileConfigCtrl *this, char *key);

#endif /* FILECONFIGCTRL_H_ */
