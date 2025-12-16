#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 학생 구조체 정의 (연결 리스트)
typedef struct Student {
    char name[20];
    int kor;
    int eng;
    int math;
    int total;
    float avg;
    int rank;
    struct Student* next;
} Student;

// 전역: 연결 리스트의 시작
Student* head = NULL;

// 학생 추가
void AddStudent(const char* name, int k, int e, int m) {
    Student* newNode = (Student*)malloc(sizeof(Student));
    if (newNode == NULL) return; // 메모리 할당 실패

    strcpy(newNode->name, name);
    newNode->kor = k;
    newNode->eng = e;
    newNode->math = m;
    newNode->total = k + e + m;
    newNode->avg = newNode->total / 3.0f;
    newNode->rank = 1;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    }
    else {
        Student* p = head;
        while (p->next != NULL)
            p = p->next;
        p->next = newNode;
    }
}

// 등수 계산
void CalculateRank(void) {
    for (Student* p = head; p != NULL; p = p->next) {
        p->rank = 1;
        for (Student* q = head; q != NULL; q = q->next) {
            if (q->total > p->total)
                p->rank++;
        }
    }
}

// 성적 출력
void PrintStudents(void) {
    if (head == NULL) {
        printf("저장된 학생 정보가 없습니다.\n");
        return;
    }

    CalculateRank();

    printf("-------------------------------------------------------\n");
    printf("이름\t국어\t영어\t수학\t총점\t평균\t등수\n");
    printf("-------------------------------------------------------\n");

    Student* p = head;
    while (p != NULL) {
        printf("%s\t%d\t%d\t%d\t%d\t%.1f\t%d등\n",
            p->name, p->kor, p->eng, p->math,
            p->total, p->avg, p->rank);
        p = p->next;
    }
}

// 파일 저장 (.dat)
void SaveToFile(void) {
    FILE* fp = fopen("students.dat", "wb");
    if (fp == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    Student* p = head;
    while (p != NULL) {
        fwrite(p, sizeof(Student) - sizeof(Student*), 1, fp);
        p = p->next;
    }
    fclose(fp);
    printf("파일 저장 완료\n");
}

// 파일 읽기 (.dat)
void LoadFromFile(void) {
    FILE* fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("파일이 존재하지 않습니다.\n");
        return;
    }

    // 기존 리스트 삭제
    while (head != NULL) {
        Student* temp = head;
        head = head->next;
        free(temp);
    }

    while (1) {
        Student temp;
        size_t r = fread(&temp, sizeof(Student) - sizeof(Student*), 1, fp);
        if (r != 1) break;
        AddStudent(temp.name, temp.kor, temp.eng, temp.math);
    }
    fclose(fp);
    printf("파일 읽기 완료\n");
}

// 메뉴 출력
void Menu(void) {
    printf("\n[Menu]\n");
    printf("1. .dat 파일에서 데이터 읽기\n");
    printf("2. 추가 학생 정보 입력\n");
    printf("3. .dat 파일 저장\n");
    printf("4. 성적 확인\n");
    printf("5. 종료\n");
    printf("-------------------\n");
    printf("선택(1~5): ");
}

int main(void) {
    int sel;
    char name[20];
    int k, e, m;

    while (1) {
        Menu();
        scanf("%d", &sel);

        if (sel == 1) {
            LoadFromFile();
        }
        else if (sel == 2) {
            printf("이름: ");
            scanf("%s", name);
            printf("국어 점수: ");
            scanf("%d", &k);
            printf("영어 점수: ");
            scanf("%d", &e);
            printf("수학 점수: ");
            scanf("%d", &m);
            AddStudent(name, k, e, m);
        }
        else if (sel == 3) {
            SaveToFile();
        }
        else if (sel == 4) {
            PrintStudents();
        }
        else if (sel == 5) {
            break;
        }
        else {
            printf("잘못된 선택입니다.\n");
        }
    }

    // 메모리 해제
    while (head != NULL) {
        Student* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
