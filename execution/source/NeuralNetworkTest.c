/**

@file: NeuralNetworkTest.c
@class: NeuralNetworkTest

Class that tests a Neural Network

@edielson
05/10/2011
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
#define NEURAL_NETWORK_TEST_VERSION        "neural-network-test-system_T01v0r1d114030\0"

/*********************************************************
    Private constants.
 *********************************************************/
#define		MAX_STRING_SIZE				30


/*********************************************************
    Private member functions declaration.
 *********************************************************/

/*********************************************************
    Member methods implementation.
 *********************************************************/

uInt16 NeuralNetworkTest(void)
{
    struct fann*			ann;
    struct fann_train_data*	ftdTestData;
    fann_type*				ftCalcOut;

    pInt8					pcParam;
    uInt16					uiNetworkInput;
    uInt16					uiNetworkOutput;
    uInt8					szTestFile[MAX_STRING_SIZE];
    uInt8					szNetworkFile[MAX_STRING_SIZE];

    uInt16 					uiCountI,uiCountJ;
	float 					fValMaximo;
	uInt16					uiClasse,uiClasseCerta;
	float 					fNumErros = 0;
	float					fQuantData;
	float					fPorcentage;

    /*Reading Neural Network Parameters*/
    ttag_FileConfigCtrl *tag_FileConfigCtrl = FileConfigCtrl_new(DEFAULT_TRAIN_CONFIG_FILE);
    
    pcParam = malloc(MAX_STRING_SIZE);
    if(pcParam)
    {
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_INPUT);
		uiNetworkInput = atoi(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_OUTPUT);
		uiNetworkOutput = atoi(pcParam);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_INPUT_FILE);
		strncpy((pInt8)szTestFile,pcParam,MAX_STRING_SIZE);
		pcParam = FileConfigCtrl_getValue(tag_FileConfigCtrl,PARAMETER_NEURAL_NETWORK_STRUCT_FILE);
		strncpy((pInt8)szNetworkFile,pcParam,MAX_STRING_SIZE);
	}

    /*Initializes Error Matrix*/
    uInt16 	uiErrorMatrix[uiNetworkOutput][uiNetworkOutput];

    for(uiCountI=0;uiCountI < uiNetworkOutput; uiCountI++)
    {	for(uiCountJ=0; uiCountJ < uiNetworkOutput; uiCountJ++)
		{
			uiErrorMatrix[uiCountI][uiCountJ]=0;
		}
    }
    /*Loading Neural Network*/
    printf("\nLoading Neural Network...");
    ann = fann_create_from_file((const char*)szNetworkFile);
    printf("Success\n");
    fann_print_parameters(ann);
    
    /*Testing*/
    printf("\nTesting...\n");
    ftdTestData = fann_read_train_from_file((const char*)szTestFile);
    uiClasseCerta=1;

   	fQuantData = fann_length_train_data(ftdTestData);
    for(uiCountI = 0; uiCountI < fQuantData; uiCountI++)
    {
   		ftCalcOut = fann_run(ann, ftdTestData->input[uiCountI]);
   		fValMaximo = ftCalcOut[0];
		uiClasse = 1;

		for (uiCountJ=1; uiCountJ < uiNetworkOutput ; uiCountJ++)
		{
			if(ftCalcOut[uiCountJ] > fValMaximo)
			{
				fValMaximo = ftCalcOut[uiCountJ];
				uiClasse = uiCountJ+1;
			}
		}

		if (uiClasse != uiClasseCerta)
		{
			fNumErros++;
			uiErrorMatrix[uiClasse-1][uiClasseCerta-1]+=1;
		}

		uiClasseCerta++;
		if (uiClasseCerta > uiNetworkOutput)
		{
			uiClasseCerta = 1;
		}
	}

    printf("Number of input data for test: %f\n", fQuantData);
	printf("Number of errors: %f\n",fNumErros);
	fPorcentage =  (float) ((fQuantData - fNumErros)/fQuantData)*100;
	printf("Percentage: %f\n",fPorcentage);

	printf("Error Matrix\n");
	for(uiCountI=0; uiCountI < uiNetworkOutput; uiCountI++)
	{
		for(uiCountJ=0; uiCountJ < uiNetworkOutput; uiCountJ++)
		{
			if(uiErrorMatrix[uiCountI][uiCountJ]!=0)
			{
				printf("Error  %2.2d  %2.2d  %2.2d\n",uiCountI+1,uiCountJ+1,uiErrorMatrix[uiCountI][uiCountJ]);
			}
		}
	}

    fann_destroy(ann);
    fann_destroy_train(ftdTestData);
    
    return 0;
}
