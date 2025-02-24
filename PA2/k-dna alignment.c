#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_K 5
#define MAX_LEN 120

int max(int a, int b){
    if(a < b){
        return b;
    }
    else{
        return a;
    }
}

void k_2_lcs(char **arr, FILE *output){
    char *arr1 = arr[0];
    char *arr2 = arr[1];
    int len1 = strlen(arr1);
    int len2 = strlen(arr2);
    int i, j;

    // calculate the length of lcs
    int **dp = (int **)malloc((len1 + 1) * sizeof(int *));

    for (int i = 0; i <= len1; i++) {
        dp[i] = (int *)malloc((len2 + 1) * sizeof(int));
    }

    for (i=0; i<=len1; i++){
        for (j=0; j<=len2; j++){
            if (i==0 || j == 0){
                dp[i][j] = 0;
            }
            else if (arr1[i-1] == arr2[j-1]){
                dp[i][j] = dp[i-1][j-1] + 1;
            }
            else{
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }

    // compute the lcs
    int idx = dp[len1][len2]; // length of lcs
    char lcs[idx+1];
    lcs[idx] = '\0';

    i = len1, j = len2;

    while (i>0 && j>0){
        if (arr1[i-1] == arr2[j-1]){
            lcs[--idx] = arr1[i-1];
            i--;
            j--;
        }
        else if (dp[i-1][j] > dp[i][j-1]){
            i--;
        }
        else{
            j--;
        }
    }

    // align and print the lcs
    char align1[len1 + len2 + 1];
    char align2[len1 + len2 + 1];
    char match[len1 + len2 + 1];

    int k = 0;
    int lcs_len = dp[len1][len2];
    i = 0, j = 0, idx = 0;

    while (i < len1 || j < len2 || idx < lcs_len) {
        if (i < len1 && j < len2 && idx < lcs_len && arr1[i] == arr2[j] && arr1[i] == lcs[idx]) {
            align1[k] = arr1[i];
            align2[k] = arr2[j];
            match[k] = '*';
            i++, j++, idx++, k++;
        }
        else if (i < len1 && (idx >= lcs_len || arr1[i] != lcs[idx])) {
            align1[k] = arr1[i];
            align2[k] = '-';
            match[k] = ' ';
            i++, k++;
        }
        else if (j < len2 && (idx >= lcs_len || arr2[j] != lcs[idx])) {
            align1[k] = '-';
            align2[k] = arr2[j];
            match[k] = ' ';
            j++, k++;
        }
    }
    align1[k] = align2[k] = match[k] = '\0';

    fprintf(output, "%s\n", align1);
    fprintf(output, "%s\n", align2);
    fprintf(output, "%s\n", match);

    for (i=0; i <= len1; i++){
        free(dp[i]);
    }
    free(dp);
}

void k_3_lcs(char **arr, FILE *output){
    char *arr1 = arr[0];
    char *arr2 = arr[1];
    char *arr3 = arr[2];
    int len1 = strlen(arr1);
    int len2 = strlen(arr2);
    int len3 = strlen(arr3);

    int i, j, k;

    // calculate the length of the lcs
    int ***dp = (int ***)malloc((len1 + 1) * sizeof(int **));

    for (i = 0; i <= len1; i++){
        dp[i] = (int **)malloc((len2 + 1) * sizeof(int *));

        for (j = 0; j <= len2; j++){
            dp[i][j] = (int *)malloc((len3 + 1) * sizeof(int));
        }
    }

    for (i = 0; i <= len1; i++){
        for (j = 0; j <= len2; j++){
            for (k = 0; k <= len3; k++){
                if (i == 0 || j == 0 || k == 0){
                    dp[i][j][k] = 0;
                }
                else if (arr1[i - 1] == arr2[j - 1] && arr1[i - 1] == arr3[k - 1]){
                    dp[i][j][k] = dp[i - 1][j - 1][k - 1] + 1;
                }
                else{
                    dp[i][j][k] = max(dp[i - 1][j][k], max(dp[i][j - 1][k], dp[i][j][k - 1]));
                }
            }
        }
    }

    // compute lcs
    int lcs_len = dp[len1][len2][len3];
    char lcs[lcs_len + 1];
    lcs[lcs_len] = '\0';

    i = len1, j = len2, k = len3;

    while (i > 0 && j > 0 && k > 0){
        // all same
        if (arr1[i - 1] == arr2[j - 1] && arr1[i - 1] == arr3[k - 1]) {
            lcs[--lcs_len] = arr1[i - 1];
            i--, j--, k--;
        }
        else if (dp[i - 1][j][k] >= dp[i][j - 1][k] && dp[i - 1][j][k] >= dp[i][j][k - 1]){
            i--;
        }
        else if (dp[i][j - 1][k] >= dp[i][j][k - 1]){
            j--;
        }
        else{
            k--;
        }
    }

    char align1[len1 + len2 + len3 + 1];
    char align2[len1 + len2 + len3 + 1];
    char align3[len1 + len2 + len3 + 1];
    char match[len1 + len2 + len3 + 1];

    int idx = 0;
    int lcs_idx = 0;
    i = 0, j = 0, k = 0;

    while (i < len1 || j < len2 || k < len3){
        // all match
        if (i < len1 && j < len2 && k < len3 && lcs_idx < strlen(lcs) &&
            arr1[i] == arr2[j] && arr1[i] == arr3[k] && arr1[i] == lcs[lcs_idx]){
            align1[idx] = arr1[i];
            align2[idx] = arr2[j];
            align3[idx] = arr3[k];
            match[idx] = '*';
            i++, j++, k++, lcs_idx++, idx++;
        }
        // i is wrong
        else if (i < len1 && (lcs_idx >= strlen(lcs) || arr1[i] != lcs[lcs_idx])){
            align1[idx] = arr1[i];
            align2[idx] = '-';
            align3[idx] = '-';
            match[idx] = ' ';
            i++, idx++;
        }
        // j is wrong
        else if (j < len2 && (lcs_idx >= strlen(lcs) || arr2[j] != lcs[lcs_idx])){
            align1[idx] = '-';
            align2[idx] = arr2[j];
            align3[idx] = '-';
            match[idx] = ' ';
            j++, idx++;
        }
        // k is wrong
        else if (k < len3 && (lcs_idx >= strlen(lcs) || arr3[k] != lcs[lcs_idx])){
            align1[idx] = '-';
            align2[idx] = '-';
            align3[idx] = arr3[k];
            match[idx] = ' ';
            k++, idx++;
        }
    }

    align1[idx] = align2[idx] = align3[idx] = match[idx] = '\0';

    fprintf(output, "%s\n", align1);
    fprintf(output, "%s\n", align2);
    fprintf(output, "%s\n", align3);
    fprintf(output, "%s\n", match);

    for (i = 0; i <= len1; i++){
        for (j = 0; j <= len2; j++){
            free(dp[i][j]);
        }
        free(dp[i]);
    }
    free(dp);
}

void k_4_lcs(char **arr, FILE *output){
    char *arr1 = arr[0];
    char *arr2 = arr[1];
    char *arr3 = arr[2];
    char *arr4 = arr[3];
    int len1 = strlen(arr1);
    int len2 = strlen(arr2);
    int len3 = strlen(arr3);
    int len4 = strlen(arr4);

    int i, j, k, l;

    // calculate the length of the lcs
    int ****dp = (int ****)malloc((len1 + 1) * sizeof(int ***));

    for (i = 0; i <= len1; i++){
        dp[i] = (int ***)malloc((len2 + 1) * sizeof(int **));

        for (j = 0; j <= len2; j++){
            dp[i][j] = (int **)malloc((len3 + 1) * sizeof(int *));

            for (k = 0; k <= len3; k++){
                dp[i][j][k] = (int *)malloc((len4 + 1) * sizeof(int));
            }
        }
    }

    for (i = 0; i <= len1; i++){
        for (j = 0; j <= len2; j++){
            for (k = 0; k <= len3; k++){
                for (l = 0; l <= len4; l++){
                    if (i == 0 || j == 0 || k == 0 || l == 0) {
                        dp[i][j][k][l] = 0;
                    }
                    else if (arr1[i - 1] == arr2[j - 1] && arr1[i - 1] == arr3[k - 1] && arr1[i - 1] == arr4[l - 1]){
                        dp[i][j][k][l] = dp[i - 1][j - 1][k - 1][l - 1] + 1;
                    }
                    else {
                        dp[i][j][k][l] = max(max(dp[i - 1][j][k][l], dp[i][j - 1][k][l]), max(dp[i][j][k - 1][l], dp[i][j][k][l - 1]));
                    }
                }
            }
        }
    }

    // compute lcs
    int lcs_len = dp[len1][len2][len3][len4];
    char lcs[lcs_len + 1];
    lcs[lcs_len] = '\0';

    i = len1, j = len2, k = len3, l = len4;

    while (i > 0 && j > 0 && k > 0 && l > 0) {
        // same
        if (arr1[i - 1] == arr2[j - 1] && arr1[i - 1] == arr3[k - 1] && arr1[i - 1] == arr4[l - 1]){
            lcs[--lcs_len] = arr1[i - 1];
            i--, j--, k--, l--;
        }
        // i > j, k, l
        else if (dp[i - 1][j][k][l] >= dp[i][j - 1][k][l] && dp[i - 1][j][k][l] >= dp[i][j][k - 1][l] && dp[i - 1][j][k][l] >= dp[i][j][k][l - 1]){
            i--;
        }
        // j > k, l
        else if (dp[i][j - 1][k][l] >= dp[i][j][k - 1][l] && dp[i][j - 1][k][l] >= dp[i][j][k][l - 1]){
            j--;
        }
        // k > l
        else if (dp[i][j][k - 1][l] >= dp[i][j][k][l - 1]){
            k--;
        }
        else{
            l--;
        }
    }

    char align1[len1 + len2 + len3 + len4 + 1];
    char align2[len1 + len2 + len3 + len4 + 1];
    char align3[len1 + len2 + len3 + len4 + 1];
    char align4[len1 + len2 + len3 + len4 + 1];
    char match[len1 + len2 + len3 + len4 + 1];

    int idx = 0;
    int lcs_idx = 0;
    i = 0, j = 0, k = 0, l = 0;

    while (i < len1 || j < len2 || k < len3 || l < len4) {
        // all same
        if (i < len1 && j < len2 && k < len3 && l < len4 && lcs_idx < strlen(lcs) &&
            arr1[i] == arr2[j] && arr1[i] == arr3[k] && arr1[i] == arr4[l] && arr1[i] == lcs[lcs_idx]){
            align1[idx] = arr1[i];
            align2[idx] = arr2[j];
            align3[idx] = arr3[k];
            align4[idx] = arr4[l];
            match[idx] = '*';
            i++, j++, k++, l++, lcs_idx++, idx++;
        }
        // i is wrong
        else if (i < len1 && (lcs_idx >= strlen(lcs) || arr1[i] != lcs[lcs_idx])){
            align1[idx] = arr1[i];
            align2[idx] = '-';
            align3[idx] = '-';
            align4[idx] = '-';
            match[idx] = ' ';
            i++, idx++;
        }
        // j is wrong
        else if (j < len2 && (lcs_idx >= strlen(lcs) || arr2[j] != lcs[lcs_idx])){
            align1[idx] = '-';
            align2[idx] = arr2[j];
            align3[idx] = '-';
            align4[idx] = '-';
            match[idx] = ' ';
            j++, idx++;
        }
        // k is wrong
        else if (k < len3 && (lcs_idx >= strlen(lcs) || arr3[k] != lcs[lcs_idx])){
            align1[idx] = '-';
            align2[idx] = '-';
            align3[idx] = arr3[k];
            align4[idx] = '-';
            match[idx] = ' ';
            k++, idx++;
        }
        // l is wrong
        else if (l < len4 && (lcs_idx >= strlen(lcs) || arr4[l] != lcs[lcs_idx])) {
            align1[idx] = '-';
            align2[idx] = '-';
            align3[idx] = '-';
            align4[idx] = arr4[l];
            match[idx] = ' ';
            l++, idx++;
        }
    }

    align1[idx] = align2[idx] = align3[idx] = align4[idx] = match[idx] = '\0';

    fprintf(output, "%s\n", align1);
    fprintf(output, "%s\n", align2);
    fprintf(output, "%s\n", align3);
    fprintf(output, "%s\n", align4);
    fprintf(output, "%s\n", match);

    for (i = 0; i <= len1; i++){
        for (j = 0; j <= len2; j++){
            for (k = 0; k <= len3; k++){
                free(dp[i][j][k]);
            }
            free(dp[i][j]);
        }
        free(dp[i]);
    }
    free(dp);
}

void k_5_lcs(char **arr, FILE *output){
    char *arr1 = arr[0];
    char *arr2 = arr[1];
    char *arr3 = arr[2];
    char *arr4 = arr[3];
    char *arr5 = arr[4];
    int len1 = strlen(arr1);
    int len2 = strlen(arr2);
    int len3 = strlen(arr3);
    int len4 = strlen(arr4);
    int len5 = strlen(arr5);

    int i, j, k, l, m;

    // calculate the length of the lcs
    int *****dp = (int *****)malloc((len1 + 1) * sizeof(int ****));

    for (i = 0; i <= len1; i++){
        dp[i] = (int ****)malloc((len2 + 1) * sizeof(int ***));

        for (j = 0; j <= len2; j++){
            dp[i][j] = (int ***)malloc((len3 + 1) * sizeof(int **));

            for (k = 0; k <= len3; k++){
                dp[i][j][k] = (int **)malloc((len4 + 1) * sizeof(int *));

                for (l = 0; l <= len4; l++){
                    dp[i][j][k][l] = (int *)malloc((len5 + 1) * sizeof(int));
                }
            }
        }
    }

    for (i = 0; i <= len1; i++){
        for (j = 0; j <= len2; j++){
            for (k = 0; k <= len3; k++){
                for (l = 0; l <= len4; l++){
                    for (m = 0; m <= len5; m++){
                        if (i == 0 || j == 0 || k == 0 || l == 0 || m == 0){
                            dp[i][j][k][l][m] = 0;
                        }
                        else if (arr1[i - 1] == arr2[j - 1] && arr1[i - 1] == arr3[k - 1] &&
                                   arr1[i - 1] == arr4[l - 1] && arr1[i - 1] == arr5[m - 1]){
                            dp[i][j][k][l][m] = dp[i - 1][j - 1][k - 1][l - 1][m - 1] + 1;
                        }
                        else{
                            dp[i][j][k][l][m] = max(max(dp[i - 1][j][k][l][m], dp[i][j - 1][k][l][m]), max(dp[i][j][k - 1][l][m], max(dp[i][j][k][l - 1][m], dp[i][j][k][l][m - 1])));
                        }
                    }
                }
            }
        }
    }

    // compute lcs
    int lcs_len = dp[len1][len2][len3][len4][len5];
    char lcs[lcs_len + 1];
    lcs[lcs_len] = '\0';

    i = len1, j = len2, k = len3, l = len4, m = len5;

    while (i > 0 && j > 0 && k > 0 && l > 0 && m > 0){
        // same
        if (arr1[i - 1] == arr2[j - 1] && arr1[i - 1] == arr3[k - 1] && arr1[i - 1] == arr4[l - 1] && arr1[i - 1] == arr5[m - 1]){
            lcs[--lcs_len] = arr1[i - 1];
            i--, j--, k--, l--, m--;
        }
        // i > j, k, l, m
        else if (dp[i - 1][j][k][l][m] >= dp[i][j - 1][k][l][m] &&
                   dp[i - 1][j][k][l][m] >= dp[i][j][k - 1][l][m] &&
                   dp[i - 1][j][k][l][m] >= dp[i][j][k][l - 1][m] &&
                   dp[i - 1][j][k][l][m] >= dp[i][j][k][l][m - 1]){
            i--;
        }
        // j > k, l, m
        else if (dp[i][j - 1][k][l][m] >= dp[i][j][k - 1][l][m] &&
                   dp[i][j - 1][k][l][m] >= dp[i][j][k][l - 1][m] &&
                   dp[i][j - 1][k][l][m] >= dp[i][j][k][l][m - 1]){
            j--;
        }
        // k > l, m
        else if (dp[i][j][k - 1][l][m] >= dp[i][j][k][l - 1][m] &&
                   dp[i][j][k - 1][l][m] >= dp[i][j][k][l][m - 1]){
            k--;
        }
        // l> m
        else if (dp[i][j][k][l - 1][m] >= dp[i][j][k][l][m - 1]){
            l--;
        }
        else{
            m--;
        }
    }

    // align and print lcs
    char align1[len1 + len2 + len3 + len4 + len5 + 1];
    char align2[len1 + len2 + len3 + len4 + len5 + 1];
    char align3[len1 + len2 + len3 + len4 + len5 + 1];
    char align4[len1 + len2 + len3 + len4 + len5 + 1];
    char align5[len1 + len2 + len3 + len4 + len5 + 1];
    char match[len1 + len2 + len3 + len4 + len5 + 1];

    int idx = 0;
    int lcs_idx = 0;
    i = 0, j = 0, k = 0, l = 0, m = 0;

    while (i < len1 || j < len2 || k < len3 || l < len4 || m < len5){
        // all same
        if (i < len1 && j < len2 && k < len3 && l < len4 && m < len5 && lcs_idx < strlen(lcs)
        && arr1[i] == arr2[j] && arr1[i] == arr3[k] && arr1[i] == arr4[l] && arr1[i] == arr5[m] && arr1[i] == lcs[lcs_idx]){
            align1[idx] = arr1[i];
            align2[idx] = arr2[j];
            align3[idx] = arr3[k];
            align4[idx] = arr4[l];
            align5[idx] = arr5[m];
            match[idx] = '*';
            i++, j++, k++, l++, m++, lcs_idx++, idx++;
        }
        // i is wrong
        else if (i < len1 && (lcs_idx >= strlen(lcs) || arr1[i] != lcs[lcs_idx])){
            align1[idx] = arr1[i];
            align2[idx] = '-';
            align3[idx] = '-';
            align4[idx] = '-';
            align5[idx] = '-';
            match[idx] = ' ';
            i++, idx++;
        }
        // j is wrong
        else if (j < len2 && (lcs_idx >= strlen(lcs) || arr2[j] != lcs[lcs_idx])){
            align1[idx] = '-';
            align2[idx] = arr2[j];
            align3[idx] = '-';
            align4[idx] = '-';
            align5[idx] = '-';
            match[idx] = ' ';
            j++, idx++;
        }
        // k is wrong
        else if (k < len3 && (lcs_idx >= strlen(lcs) || arr3[k] != lcs[lcs_idx])){
            align1[idx] = '-';
            align2[idx] = '-';
            align3[idx] = arr3[k];
            align4[idx] = '-';
            align5[idx] = '-';
            match[idx] = ' ';
            k++, idx++;
        }
        // l is wrong
        else if (l < len4 && (lcs_idx >= strlen(lcs) || arr4[l] != lcs[lcs_idx])){
            align1[idx] = '-';
            align2[idx] = '-';
            align3[idx] = '-';
            align4[idx] = arr4[l];
            align5[idx] = '-';
            match[idx] = ' ';
            l++, idx++;
        }
        // m is wrong
        else if (m < len5 && (lcs_idx >= strlen(lcs) || arr5[m] != lcs[lcs_idx])){
            align1[idx] = '-';
            align2[idx] = '-';
            align3[idx] = '-';
            align4[idx] = '-';
            align5[idx] = arr5[m];
            match[idx] = ' ';
            m++, idx++;
        }
    }

    align1[idx] = align2[idx] = align3[idx] = align4[idx] = align5[idx] = match[idx] = '\0';

    fprintf(output, "%s\n", align1);
    fprintf(output, "%s\n", align2);
    fprintf(output, "%s\n", align3);
    fprintf(output, "%s\n", align4);
    fprintf(output, "%s\n", align5);
    fprintf(output, "%s\n", match);

    for (i = 0; i <= len1; i++){
        for (j = 0; j <= len2; j++){
            for (k = 0; k <= len3; k++){
                for (l = 0; l<=len4; l++){
                    free(dp[i][j][k][l]);
                }
                free(dp[i][j][k]);
            }
            free(dp[i][j]);
        }
        free(dp[i]);
    }
    free(dp);
}

int main(){
    int k, i;

    FILE *input = fopen("hw2_input.txt", "r");
    FILE *output = fopen("hw2_output.txt", "w");

    fscanf(input, "%d", &k);
    fscanf(input, "%*s");

    char *arr[k];

    for (i=0; i<k; i++){
        arr[i] = (char *)malloc(sizeof(char) * MAX_LEN);
        fscanf(input, "%s", arr[i]);
    }

    if (k == 2){
        k_2_lcs(arr, output);
    }
    else if (k == 3){
        k_3_lcs(arr, output);
    }
    else if (k == 4){
        k_4_lcs(arr, output);
    }
    else if (k == 5){
        k_5_lcs(arr, output);
    }
    else{
        return -1;
    }

    fclose(input);
    fclose(output);

    for (int i = 0; i < k; i++) {
        free(arr[i]);
    }

    return 0; 
}
