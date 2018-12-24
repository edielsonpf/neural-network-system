/**

@file: NeuralNetworkTrain.c
@class: NeuralNetworkTrain

Class that trains a Neural Network

@edielson
25/09/2011
*/

#include <floatfann.h>
#include <string.h>
#include "FileConfigCtrl.h"

/***********************************************************************
 * Version control
 *
 * <projeto>_<subprojeto>_<branch>v<versão>r<release>d<ano><semana><dia><delivery>
 *
 * Onde:
 * <projeto> é o nome do projeto (ex: SAT);
 * <subprojeto> é o nome do subprojeto (ex: ACInterface);
 * <branch> é o ramo do produto (ex: T01);
 * <versão> é o número da versão (ex: 0);
 * <release> é o número do release (ex:1);
 * <ano> é o ano com duas casas (ex:11);
 * <semana> é o número da semana no ano, com duas casas ISO8601 (pode ser visto no Outlook - 1) (ex:12);
 * <dia> é o dia da semana onde domingo é 0, segunda é 1 até sábado que é 6 (ex: 3);
 * <delivery> é o número do delivery dentro do dia em hexa, com uma casa, iniciando em 0 (ex: 0).
 *
 **********************************************************************/
#define NEURAL_NETWORK_TRAIN_VERSION        "neural-network-train-system_T01v0r1d114020\0"

/*********************************************************
    Private constants.
 *********************************************************/
#define		MAX_STRING_SIZE				30
#define 	NEURAL_NETWORK_MANUAL		"manual"
#define 	NEURAL_NETWORK_AUTOMATIC	"automatic"

/*********************************************************
    Private member functions declaration.
 *********************************************************/

/*********************************************************
    Member methods implementation.
 *********************************************************/

uInt16 NeuralNetworkTrain(void)
{
    struct fann*		ann;
    struct fann_train_data*	fdtTrainData;
    uInt32			ulEpoch;
    pInt8			pcParam;

    uInt16			uiNetworkInput;
    uInt16			uiNetworkOutput;
    uInt16			uiNetworkHidden;
    uInt16			uiNetworkLayers;
    uInt32			ulMaxTrainEpochs;
    uInt32			ulReportTrainEpochs;
    float			fTrainError;
    float			fLearningRate;
    float			fDecLearningRate;
    uInt8			szTrainFile[MAX_STRING_SIZE];
    uInt8			szTrainOutputFile[MAX_STRING_SIZE];
    uInt8			szTrainMethod[MAX_STRING_SIZE];

    float			fError;
    float			fMseError;
    uInt32			ulReportEpoch;
    
    /*Reading Neural Network Parameters*/
    ttag_FileConfigCtrl *tag_FileConfigCtrl = FileConfigCtrl_new(DEFAULT_TRAIN_CONFIG_FILE);
    
    pcParam = malloc(MAX_STRING_SIZE);
    if(pcParam)
    {
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_INPUT);
		uiNetworkInput = atoi(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_OUTPUT);
		uiNetworkOutput = atoi(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_LAYERS);
		uiNetworkLayers = atoi(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_HIDDEN_NEURONS);
		uiNetworkHidden = atoi(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_MAX_EPOCHS);
		ulMaxTrainEpochs = atol(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_REPORTS_EPOCHS);
		ulReportTrainEpochs = atol(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_ERROR);
		fTrainError = atof(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_LEARNING_RATE);
		fLearningRate = atof(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_INPUT_FILE);
		strncpy((pInt8)szTrainFile,pcParam,MAX_STRING_SIZE);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_OUTPUT_FILE);
		strncpy((pInt8)szTrainOutputFile,pcParam,MAX_STRING_SIZE);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_DECREASE_LR);
		fDecLearningRate = atof(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_METHOD);
		strncpy((pInt8)szTrainMethod,pcParam,MAX_STRING_SIZE);
    }

    /*Creatin Neural Network*/
    printf("\nCreating Neural Network...");
    ann = fann_create_standard(uiNetworkLayers, uiNetworkInput, uiNetworkHidden, uiNetworkOutput);
    /*Setting activation functions*/
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
    fann_randomize_weights(ann,-0.77,0.77);
    fann_set_learning_rate(ann,fLearningRate);
    printf("Success\n");
    fann_print_parameters(ann);
    

    /*Training*/
    printf("\nTraining...\n");
    fdtTrainData = fann_read_train_from_file((const char*)szTrainFile);
    fann_shuffle_train_data(fdtTrainData);

    if(strncmp((const char*)szTrainMethod,NEURAL_NETWORK_MANUAL,MAX_STRING_SIZE) == 0)
    {
		ulReportEpoch = ulReportTrainEpochs;

		printf("MaxEpochs	%5.0ld.	Desired Error: %2.8f.\n",ulMaxTrainEpochs,fTrainError);
		for(ulEpoch = 1; ulEpoch <= ulMaxTrainEpochs;ulEpoch++)
		{
			fError = fann_train_epoch(ann,fdtTrainData);

			if(ulEpoch == ulReportEpoch)
			{
				printf("Epoch		%5.0ld.	Current Error: %2.8f.\n",ulEpoch,fError);
				ulReportEpoch+=ulReportTrainEpochs;
			}

			if(fError < fTrainError)
			{
				break;
			}

			fLearningRate*=fDecLearningRate;

			if(fLearningRate < 0.00001)
			{
				fLearningRate = 0.00001;
			}
	
			fLearningRate*=fDecLearningRate;
			fann_set_learning_rate(ann,fLearningRate);
		}
    }
    else if(strncmp((const char*)szTrainMethod,NEURAL_NETWORK_AUTOMATIC,MAX_STRING_SIZE) == 0)
    {
    	fann_train_on_data(ann,fdtTrainData,ulMaxTrainEpochs,ulReportTrainEpochs,fTrainError);
    }
    /*Testing Neural Network*/
    printf("\nTesting Neural Network...");
    fann_reset_MSE(ann);
	fann_test_data(ann,fdtTrainData);
	fMseError = fann_get_MSE(ann);
	printf("\nMSE: %2.8f.\n",fMseError);

    /*Saving Neural Network*/
    printf("\nSaving Neural Network...");
    fann_save(ann, (const char*)szTrainOutputFile);
    printf("Success\n");

    fann_destroy(ann);
    fann_destroy_train(fdtTrainData);
    
    return 0;
}
