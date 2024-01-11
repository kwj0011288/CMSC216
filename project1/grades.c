/*
author: Wonjae Kim
UID: 117283365
Directory ID: wkim1128
*/

#include <stdio.h>
#include <math.h>
#define MAX_LENGTH 50 /* maximum length of the array */

int check_order(int number_of_assignment, int assignment_num[],
                int assignment_score[], int assignment_weight[],
                int assignment_late[]);

void numeric_score(int number_of_assignment, int assignment_score[],
                   int assignment_weight[], int assignment_late[],
                   int penalty_point_per_day, int drop);

void drop_assignment(int number_of_assignment, int assignment_num[],
                     int assignment_score[], int assignment_weight[],
                     int assignment_late[], int penalty_point_per_day,
                     int drop);

void stat(char check, int number_of_assignment, int assignment_score[],
          int assignment_weight[], int assignment_late[],
          int penalty_point_per_day);

double get_mean(int number_of_assignment, int assignment_score[],
                int assignment_weight[], int assignment_late[],
                int penalty_point_per_day);

double get_deviation(int number_of_assignment, int assignment_score[],
                     int assignment_weight[], int assignment_late[],
                     int penalty_point_per_day);

int main() {

    int penalty_point_per_day, assignment_drop, number_of_assignment;

    int number, score, weight, late, i, weight_sum;

    char stats;

    int assignment_num[MAX_LENGTH];

    int assignment_score[MAX_LENGTH];

    int assignment_weight[MAX_LENGTH];

    int assignment_late[MAX_LENGTH];

    scanf("%d %d %c", &penalty_point_per_day, &assignment_drop, &stats);

    scanf("%d", &number_of_assignment);

    for (i = 0; i < number_of_assignment; i++) {
        scanf("%d, %d, %d, %d", &number, &score, &weight, &late);
        assignment_num[i] = number;
        assignment_score[i] = score;
        assignment_weight[i] = weight;
        assignment_late[i] = late;
        weight_sum += assignment_weight[i];
    }

    if (weight_sum != 100) {

        printf("ERROR: Invalid values provided\n");
        
    } else {

        /* get grades and print out the numeric score*/
        drop_assignment(number_of_assignment, assignment_num, assignment_score,
                        assignment_weight, assignment_late, penalty_point_per_day,
                        assignment_drop);

        printf("Points Penalty Per Day Late: %d\n", penalty_point_per_day);

        printf("Number of Assignments Dropped: %d\n", assignment_drop);

        printf("Values Provided: \n");

        printf("Assignment, Score, Weight, Days Late\n");

        check_order(number_of_assignment, assignment_num, assignment_score,
                    assignment_weight, assignment_late);

        for (i = 0; i < number_of_assignment; i++) {
            printf("%d, %d, %d, %d\n", assignment_num[i], assignment_score[i],
                   assignment_weight[i], assignment_late[i]);
        }

        if (stats == 'y' || stats == 'Y') {
            stat(stats, number_of_assignment, assignment_score, assignment_weight,
                 assignment_late, penalty_point_per_day);
        }
    }
}

int check_order(int number_of_assignment, int assignment_num[], int assignment_score[],
                int assignment_weight[], int assignment_late[]) {

    int i, j, temp;
    /* check assignment numbers to sort them in order */
    for (i = 0; i < number_of_assignment - 1; i++) {
        for (j = 0; j < number_of_assignment - i - 1; j++) {
            if (assignment_num[j] > assignment_num[j + 1]) {

                temp = assignment_num[j];
                assignment_num[j] = assignment_num[j + 1];
                assignment_num[j + 1] = temp;

                temp = assignment_score[j];
                assignment_score[j] = assignment_score[j + 1];
                assignment_score[j + 1] = temp;

                temp = assignment_weight[j];
                assignment_weight[j] = assignment_weight[j + 1];
                assignment_weight[j + 1] = temp;

                temp = assignment_late[j];
                assignment_late[j] = assignment_late[j + 1];
                assignment_late[j + 1] = temp;
            }
        }
    }
    return 0;
}

void drop_assignment(int number_of_assignment, int assignment_num[],
                     int assignment_score[], int assignment_weight[],
                     int assignment_late[], int penalty_point_per_day,
                     int drop) {

    int i, j, temp;
    /* sort it descending order to put drop assignment at the bottom */
    for (i = 0; i < number_of_assignment - 1; i++) {
        for (j = 0; j < number_of_assignment - i - 1; j++) {

            if (assignment_score[j] * assignment_weight[j] <
                    assignment_score[j + 1] * assignment_weight[j + 1] ||
                (assignment_score[j] * assignment_weight[j] ==
                     assignment_score[j + 1] * assignment_weight[j + 1] &&
                 assignment_num[j] < assignment_num[j + 1])) {

                temp = assignment_score[j];
                assignment_score[j] = assignment_score[j + 1];
                assignment_score[j + 1] = temp;

                temp = assignment_num[j];
                assignment_num[j] = assignment_num[j + 1];
                assignment_num[j + 1] = temp;

                temp = assignment_weight[j];
                assignment_weight[j] = assignment_weight[j + 1];
                assignment_weight[j + 1] = temp;

                temp = assignment_late[j];
                assignment_late[j] = assignment_late[j + 1];
                assignment_late[j + 1] = temp;
            }
        }
    }
    /* call this method to get numeric score */
    numeric_score(number_of_assignment, assignment_score, assignment_weight,
                  assignment_late, penalty_point_per_day, drop);
}

void numeric_score(int number_of_assignment, int assignment_score[],
                   int assignment_weight[], int assignment_late[],
                   int penalty_point_per_day, int drop) {

    int i;

    double sum = 0;

    double denominator = 0;

    for (i = 0; i < number_of_assignment - drop; i++) {
        sum += (assignment_score[i] - assignment_late[i] * penalty_point_per_day) * (assignment_weight[i]);
        denominator += (assignment_weight[i]);
    }
    sum = sum / (denominator);
    printf("Numeric Score: %5.4f\n", sum);
}

double get_mean(int number_of_assignment, int assignment_score[],
                int assignment_weight[], int assignment_late[],
                int penalty_point_per_day) {

    int i, score;

    double sum = 0;

    for (i = 0; i < number_of_assignment; i++) {
        score = assignment_score[i] - (penalty_point_per_day * assignment_late[i]);
        sum += score;
    }
    return sum / number_of_assignment;
}

double get_deviation(int number_of_assignment, int assignment_score[],
                     int assignment_weight[], int assignment_late[],
                     int penalty_point_per_day) {

    int i;

    double sum = 0;

    double deviation = 0;

    double mean = get_mean(number_of_assignment, assignment_score,
                           assignment_weight, assignment_late,
                           penalty_point_per_day);

    for (i = 0; i < number_of_assignment; i++) {
        sum += pow((assignment_score[i] -
                    (penalty_point_per_day * assignment_late[i]) - mean),
                   2);
    }

    deviation = sum / number_of_assignment;

    return sqrt(deviation);
}

void stat(char check, int number_of_assignment, int assignment_score[],
          int assignment_weight[], int assignment_late[],
          int penalty_point_per_day) {

    double mean, deviation;

    if (check == 'Y' || check == 'y') {
        mean = get_mean(number_of_assignment, assignment_score, assignment_weight,
                        assignment_late, penalty_point_per_day);
        deviation = get_deviation(number_of_assignment, assignment_score,
                                  assignment_weight, assignment_late,
                                  penalty_point_per_day);
        printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean, deviation);
    }
}