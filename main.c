#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int INT_MAX = 2147483647;
const int INT_MIN = -2147483648;

const char *FILE_ADRESS = "/home/emre/CProjects/BigOProject/veri.txt";
// const char *FILE_ADRESS = "C:\\Users\\Yonis\\OneDrive\\Masaüstü\\kod.txt";
//  #region BigO time
char *codeArray;
int codeArrayLength;
int mainKeyCounter = 0;

typedef struct BracketStack BracketStack;
struct BracketStack
{
    char bracket;
    BracketStack *next;
};

typedef struct BigOStack BigOStack;
struct BigOStack
{
    int bigO;
    BigOStack *prev;
};

typedef struct BigOContainer BigOContainer;
struct BigOContainer
{
    int key;
    BigOStack *stack;
    BigOContainer *next;
};
BigOContainer *mainContainer = NULL;

BracketStack *bracketStack = NULL;

BigOStack *diskStack = NULL;
BigOStack *diskLogStack = NULL;

int charToInt(char c)
{
    return c - '0';
}

void pushBracket(char bracket)
{
    BracketStack *newStack = (BracketStack *)malloc(sizeof(BracketStack));
    newStack->bracket = bracket;
    newStack->next = bracketStack;

    bracketStack = newStack;
}

void pushStackToMainContainer(BigOStack *stack, int key)
{
    BigOContainer *newStack = (BigOContainer *)malloc(sizeof(BigOContainer));
    newStack->stack = stack;
    newStack->next = mainContainer;
    newStack->key = key;

    mainContainer = newStack;
}

int takeNewKey()
{
    mainKeyCounter += 1;
    return mainKeyCounter;
}

int popStackFromMainContainer()
{
    if (mainContainer == NULL)
    {
        return 0;
    }
    else if (mainContainer->next == NULL)
    {
        mainContainer = NULL;
    }
    else
    {
        mainContainer = mainContainer->next;
    }

    return 1;
}

BigOStack *pushBigO(int bigO, BigOStack *stacks)
{
    BigOStack *newStack = (BigOStack *)malloc(sizeof(BigOStack));
    newStack->bigO = bigO;
    newStack->prev = stacks;

    return newStack;
}

int popBigO(BigOStack *stacks)
{
    if (stacks == NULL)
    {
        return 0;
    }
    else if (stacks->prev == NULL)
    {
        stacks = NULL;
    }
    else
    {
        stacks = stacks->prev;
    }

    return 1;
}

BigOStack *copyBigOStack(BigOStack *stack)
{
    if (stack == NULL)
    {
        return NULL;
    }

    BigOStack *newStack = (BigOStack *)malloc(sizeof(BigOStack));
    newStack->bigO = stack->bigO;
    newStack->prev = copyBigOStack(stack->prev);

    return newStack;
}

int popBracket()
{
    if (bracketStack == NULL)
    {
        return 0;
    }
    else if (bracketStack->next == NULL)
    {
        bracketStack = NULL;
    }
    else
    {
        bracketStack = bracketStack->next;
    }

    return 1;
}

int isBracketStackEmpty()
{
    if (bracketStack == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int isBigOStackEmpty(BigOStack *stacks)
{
    if (stacks == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int lengthOfMainContainer()
{
    BigOContainer *containerPointer = mainContainer;
    int counter = 0;
    while (containerPointer != NULL)
    {
        counter += 1;
        containerPointer = containerPointer->next;
    }

    return counter;
}

char lastBracket()
{
    return bracketStack->bracket;
}

char reverseBracket(char data)
{
    if (data == '}')
    {
        return '{';
    }
    else if (data == ')')
    {
        return '(';
    }
    else if (data == ']')
    {
        return '[';
    }
    printf("hata 1");
    exit(1);
}

int checkForBrackets()
{
    for (int a = 0; a < codeArrayLength; a++)
    {
        if (codeArray[a] == '(' || codeArray[a] == '[' || codeArray[a] == '{')
        {
            pushBracket(codeArray[a]);
        }
        if (codeArray[a] == ')' || codeArray[a] == ']' || codeArray[a] == '}')
        {
            if (reverseBracket(codeArray[a]) == lastBracket())
            {
                if (!popBracket())
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
    }

    if (!isBracketStackEmpty() || codeArrayLength < 3)
    {
        return 0;
    }

    return 1;
}

char *takeCodeToArray()
{
    FILE *file = fopen(FILE_ADRESS, "r");

    if (file == NULL)
    {
        printf("File not found!\n");
        exit(1);
    }

    int c;
    int length = 2;
    char *buf = malloc(sizeof(char) * length);
    if (buf == NULL)
    {
        printf("\n\nERROR NO: 06\n\n");
        exit(1);
    }

    while ((c = fgetc(file)) != EOF)
    {
        if (c == ' ' || c == 10 || c == '\t')
        {
            continue;
        }

        buf = realloc(buf, sizeof(char) * length);
        if (buf == NULL)
        {
            printf("\n\nERROR NO: 05\n\n");
            exit(1);
        }

        buf[length - 2] = c;

        length += 1;
    }

    buf[length - 2] = '\0';

    fclose(file);

    codeArrayLength = length - 2;

    return buf;
}

int isFor(int startIndex)
{
    if (codeArrayLength - startIndex <= 3)
        return 0;

    if (codeArray[startIndex] == 'f' &&
        codeArray[startIndex + 1] == 'o' &&
        codeArray[startIndex + 2] == 'r' &&
        codeArray[startIndex + 3] == '(')
    {
        return 1;
    }

    return 0;
}

int isWhile(int startIndex)
{
    if (codeArrayLength - startIndex <= 5)
        return 0;

    if (codeArray[startIndex] == 'w' &&
        codeArray[startIndex + 1] == 'h' &&
        codeArray[startIndex + 2] == 'i' &&
        codeArray[startIndex + 3] == 'l' &&
        codeArray[startIndex + 4] == 'e' &&
        codeArray[startIndex + 5] == '(')
    {
        return 1;
    }

    return 0;
}

int isDoWhile(int startIndex)
{
    if (codeArray[startIndex - 1] == ';' || codeArray[startIndex - 1] == '}' || codeArray[startIndex - 1] == '{')
    {
        if (codeArray[startIndex] == 'd' &&
            codeArray[startIndex + 1] == 'o' &&
            codeArray[startIndex + 2] == '{')
        {
            return 1;
        }
    }
    return 0;
}

int isGlobalFunction(int startIndex)
{
    int bracketCounter = 0;

    for (int a = 0; a < codeArrayLength; a++)
    {
        if (codeArray[a] == '{')
        {
            bracketCounter += 1;
        }
        else if (codeArray[a] == '}')
        {
            bracketCounter -= 1;
        }

        if (bracketCounter == 0)
        {
            if (!isalpha(codeArray[a]) && a > startIndex)
            {
                if (codeArray[a] == '(')
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }

    return 0;
}

int localFunctionEndIndex(int startIndex)
{
    if (isFor(startIndex) || isWhile(startIndex))
    {

        return -1;
    }

    int leftDoor = 0;
    int alphaDoor = 1;

    if (codeArray[startIndex - 1] == '{' || codeArray[startIndex - 1] == '}' || codeArray[startIndex - 1] == ';')
    {
        for (int a = startIndex; a < codeArrayLength - 1; a++)
        {
            if (!isalpha(codeArray[a]) && alphaDoor == 1)
            {
                if (codeArray[a] != '(')
                {
                    return -1;
                }
                alphaDoor = 0;
                continue;
            }
            if (!alphaDoor)
            {
                if (codeArray[a] == ')' && codeArray[a + 1] == ';')
                {
                    return a + 1;
                }
                else if (codeArray[a] == '{' || codeArray[a] == '}')
                {
                    return -1;
                }
            }
        }
    }
    return -1;
}

int findStartOfLocalFunction(int startIndex, int endIndex, int recursiveException)
{
    for (int a = startIndex; a <= endIndex; a++)
    {
        if (localFunctionEndIndex(a) != -1 && a != recursiveException)
        {
            return a;
        }
    }

    return INT_MAX;
}

int findStartOfGlobalFunction(char *functionName)
{
    char *realFuncName = functionName;

    if (strlen(functionName) > 6)
    {
        if (functionName[0] == 'r' &&
            functionName[1] == 'e' &&
            functionName[2] == 't' &&
            functionName[3] == 'u' &&
            functionName[4] == 'r' &&
            functionName[5] == 'n')
        {
            realFuncName = (char *)malloc(sizeof(char) * strlen(functionName) - 5);
            for (int a = 6; a < strlen(functionName); a++)
            {
                realFuncName[a - 6] = functionName[a];
            }
            realFuncName[strlen(functionName) - 6] = '\0';
        }
    }

    for (int a = 0; a < codeArrayLength; a++)
    {
        if (isGlobalFunction(a))
        {
            int door = 1;
            for (int e = 0; e < strlen(realFuncName); e++)
            {
                char bir = codeArray[a + e];
                char iki = realFuncName[e];
                if (codeArray[a + e] != realFuncName[e])
                {
                    door = 0;
                    break;
                }
            }
            if (door)
            {
                return a;
            }
        }
    }

    return INT_MAX;
}

int findEndOfGlobalFunction(int startIndex)
{
    int bracketCounter = 0;

    for (int a = startIndex; a < codeArrayLength; a++)
    {
        if (codeArray[a] == '{')
        {
            bracketCounter += 1;
        }
        else if (codeArray[a] == '}')
        {
            bracketCounter -= 1;
            if (bracketCounter == 0)
            {
                return a;
            }
        }
    }

    printf("ERROR CODE: 234");
    exit(1);
}

int stringContains(int startindex, int endindex, char data)
{
    for (int a = startindex; a <= endindex; a++)
    {
        if (codeArray[a] == data)
        {
            return 1;
        }
    }
    return 0;
}
int *checkBracketOfWhile(int startIndex, int endIndex)
{
    int *array = (int *)malloc(sizeof(int) * 2);

    for (int a = startIndex; a < endIndex; a++)
    {

        if ((codeArray[a] == '<' && codeArray[a + 1] == '=') ||
            (codeArray[a] == '>' && codeArray[a + 1] == '=') ||
            (codeArray[a] == '!' && codeArray[a + 1] == '=') ||
            (codeArray[a] == '=' && codeArray[a + 1] == '='))
        {
            array[0] = a;
            array[1] = a + 1;
            break;
        }
        else if (codeArray[a] == '<' || codeArray[a] == '>')
        {
            array[0] = a;
            array[1] = -1;
            break;
        }
    }

    return array;
}

int findBigOByMark(int startIndex, int endIndex)
{
    if (stringContains(startIndex, endIndex, '+') ||
        stringContains(startIndex, endIndex, '-'))
    {
        return 0;
    }
    if (stringContains(startIndex, endIndex, '*') ||
        stringContains(startIndex, endIndex, '/'))
    {
        int containerNumber = 0;
        for (int a = startIndex; a <= endIndex; a++)
        {
            if (isdigit(codeArray[a]))
            {
                containerNumber += charToInt(codeArray[a]);
                containerNumber *= 10;
            }
        }
        return containerNumber / 10;
    }

    printf("ERROR CODE: 364");
    exit(1);
}

// returns 0 if n, positive number if log
int checkPieceOfFor(int startIndex, int finishIndex)
{
    int semicolonCounter = 0;
    int newStartIndex = -1;
    int newEndIndex = -1;

    for (int a = startIndex; a <= finishIndex; a++)
    {
        if (codeArray[a] == ';')
        {
            semicolonCounter += 1;
        }
        if (semicolonCounter == 2)
        {
            if (newStartIndex == -1)
            {
                newStartIndex = a + 1;
            }
            if (codeArray[a] == ')' && codeArray[a + 1] == '{')
            {
                newEndIndex = a - 1;
                break;
            }
        }
    }

    return findBigOByMark(newStartIndex, newEndIndex);

    printf("ERROR CODE: 365");
    exit(1);
}

int findStartOfFor(int startIndex, int finishIndex)
{
    for (int a = startIndex; a < finishIndex - 3; a++)
    {
        if (isFor(a))
        {
            return a;
        }
    }

    return INT_MAX;
}
int findStartOfWhile(int startIndex, int finishIndex)
{
    for (int a = startIndex; a <= finishIndex - 5; a++)
    {
        if (isWhile(a))
        {
            return a;
        }
    }

    return INT_MAX;
}

int findStartOfDoWhile(int startIndex, int finishIndex)
{
    for (int a = startIndex; a <= finishIndex - 3; a++)
    {
        if (isDoWhile(a))
        {
            return a;
        }
    }

    return INT_MAX;
}

int findFinishOfDoWhile(int finishIndex)
{
    int door = 0;

    for (int a = finishIndex; a < codeArrayLength - 1; a++)
    {
        if (codeArray[a] == '(')
        {
            door = 1;
        }
        if (door == 1 && codeArray[a] == ')' && codeArray[a + 1] == ';')
        {
            return a;
        }
    }

    printf("HATA 304");
    exit(1);
}

int checkPieceOfWhile(int startIndex, int finishIndex, int doWhileCodeStart, int doWhileCodeEnd)
{
    int *whileInts;
    int bracketStart = -1;
    int bracketEnd = -1;

    int endIndex = finishIndex;

    int whileCodeStartIndex = doWhileCodeStart;

    for (int a = startIndex; a <= finishIndex; a++)
    {
        if (codeArray[a] == '(')
        {
            bracketStart = a + 1;
        }
        else if (codeArray[a] == ')')
        {
            bracketEnd = a - 1;
            whileInts = checkBracketOfWhile(bracketStart, bracketEnd);
        }
        else if (codeArray[a] == '{')
        {
            whileCodeStartIndex = a;
            break;
        }
    }

    int leftMark = whileInts[0];
    int rightMark;

    if (whileInts[1] == -1)
    {
        rightMark = whileInts[0];
    }
    else
    {
        rightMark = whileInts[1];
    }

    if (doWhileCodeStart != -1)
    {
        endIndex = doWhileCodeEnd;
    }

    int leftSideLength = leftMark - bracketStart;
    int rightSideLength = bracketEnd - rightMark;
    char *leftSideString = (char *)malloc(sizeof(char) * leftSideLength);
    char *rightSideString = (char *)malloc(sizeof(char) * rightSideLength);

    if (isdigit(codeArray[bracketStart]))
    {
        // SADECE SAĞ TARAF DEĞİŞKEN
        for (int a = rightMark + 1; a <= bracketEnd; a++)
        {
            rightSideString[a - rightMark - 1] = codeArray[a];
        }

        for (int a = whileCodeStartIndex; a <= endIndex - rightSideLength; a++)
        {
            int door = 1;
            for (int e = 0; e < rightSideLength; e++)
            {
                if (codeArray[a + e] != rightSideString[e])
                {
                    door = 0;
                }
            }

            if (door)
            {
                if (!isalpha(codeArray[a - 1]))
                {
                    int indexOfSemiColon = -1;

                    for (int i = a; a < endIndex; i++)
                    {
                        if (codeArray[i] == ';')
                        {
                            indexOfSemiColon = i;
                            break;
                        }
                    }

                    return findBigOByMark(a, indexOfSemiColon);
                }
            }
        }
    }
    else if (isdigit(codeArray[bracketEnd]))
    {
        // SADECE SOL TARAF DEĞİŞKEN
        for (int a = bracketStart; a <= leftMark - 1; a++)
        {
            leftSideString[a - bracketStart] = codeArray[a];
        }

        for (int a = whileCodeStartIndex; a <= endIndex - leftSideLength; a++)
        {
            int door = 1;
            for (int e = 0; e < leftSideLength; e++)
            {
                if (codeArray[a + e] != leftSideString[e])
                {
                    door = 0;
                }
            }

            if (door)
            {
                if (!isalpha(codeArray[a - 1]))
                {
                    int indexOfSemiColon = -1;

                    for (int i = a; a < endIndex; i++)
                    {
                        if (codeArray[i] == ';')
                        {
                            indexOfSemiColon = i;
                            break;
                        }
                    }

                    return findBigOByMark(a, indexOfSemiColon);
                }
            }
        }
    }
    else
    {
        // İKİ TARAF DEĞİŞKEN
        for (int a = rightMark + 1; a <= bracketEnd; a++)
        {
            rightSideString[a - rightMark - 1] = codeArray[a];
        }

        for (int a = bracketStart; a <= leftMark - 1; a++)
        {
            leftSideString[a - bracketStart] = codeArray[a];
        }

        for (int a = whileCodeStartIndex; a <= endIndex - rightSideLength; a++)
        {
            int door = 1;
            for (int e = 0; e < rightSideLength; e++)
            {
                if (codeArray[a + e] != rightSideString[e])
                {
                    door = 0;
                }
            }

            if (door)
            {
                if (!isalpha(codeArray[a - 1]))
                {
                    int indexOfSemiColon = -1;

                    for (int i = a; a < endIndex; i++)
                    {
                        if (codeArray[i] == ';')
                        {
                            indexOfSemiColon = i;
                            break;
                        }
                    }

                    if (indexOfSemiColon != -1)
                    {
                        return findBigOByMark(a, indexOfSemiColon);
                    }
                }
            }
        }

        for (int a = whileCodeStartIndex; a <= endIndex - leftSideLength; a++)
        {
            int door = 1;
            for (int e = 0; e < leftSideLength; e++)
            {
                if (codeArray[a + e] != leftSideString[e])
                {
                    door = 0;
                }
            }

            if (door)
            {
                if (!isalpha(codeArray[a - 1]))
                {
                    int indexOfSemiColon = -1;

                    for (int i = a; a < endIndex; i++)
                    {
                        if (codeArray[i] == ';')
                        {
                            indexOfSemiColon = i;
                            break;
                        }
                    }

                    if (indexOfSemiColon != -1)
                    {
                        return findBigOByMark(a, indexOfSemiColon);
                    }
                }
            }
        }
    }

    printf("HATA KOD: 101");
    exit(1);
}

int findFinishOfLoop(int startIndex)
{
    int stackNumber = 0;

    for (int a = startIndex; a < codeArrayLength; a++)
    {
        if (codeArray[a] == '{')
        {
            stackNumber += 1;
        }
        else if (codeArray[a] == '}')
        {
            stackNumber -= 1;

            if (stackNumber == 0)
            {
                return a;
            }
        }
    }

    printf("HATA VAR!: 27");
    exit(1);
}

int stringCompareByLast(char *recFirst, char *recSecond)
{
    int firstLength = strlen(recFirst) - 1;
    int secondLength = strlen(recSecond) - 1;
    for (; firstLength >= 0 && secondLength >= 0; firstLength--, secondLength--)
    {
        if (recSecond[secondLength] != recFirst[firstLength])
        {
            return 0;
        }
    }
    return 1;
}

BigOStack *calculateBigOViaKey(int key)
{
    int lengthOfMainCon = lengthOfMainContainer();

    int *nBigO = (int *)malloc(sizeof(int) * lengthOfMainCon);
    int *logBigO = (int *)malloc(sizeof(int) * lengthOfMainCon);

    BigOContainer *containerStack = mainContainer;

    int maxIndex = 0;

    for (int a = 0; a < lengthOfMainCon; a++)
    {
        if (containerStack->key != key)
        {
            containerStack = containerStack->next;
            continue;
        }

        BigOStack *stackPointer = containerStack->stack;
        int counterBigO = 0;
        int multiplyOfLogs = 1;

        while (stackPointer != NULL)
        {
            if (stackPointer->bigO == 0)
            {
                counterBigO += 1;
            }
            else
            {
                multiplyOfLogs *= stackPointer->bigO;
            }

            stackPointer = stackPointer->prev;
        }

        nBigO[maxIndex] = counterBigO;
        logBigO[maxIndex] = multiplyOfLogs;
        containerStack = containerStack->next;
        maxIndex += 1;
    }

    int maxN = -1;
    int maxLog = 0;
    int currentIndex = -1;

    for (int a = 0; a < maxIndex; a++)
    {
        if (nBigO[a] > maxN)
        {
            maxN = nBigO[a];
            maxLog = logBigO[a];
            currentIndex = a;
        }
        else if (nBigO[a] == maxN && logBigO[a] > maxLog)
        {
            maxN = nBigO[a];
            maxLog = logBigO[a];
            currentIndex = a;
        }
    }

    containerStack = mainContainer;

    for (int a = 0; a < currentIndex; a++)
    {
        if (containerStack->key != key)
        {
            a -= 1;
        }
        containerStack = containerStack->next;
    }

    return containerStack->stack;
}

void checkForBigO(int startIndex, int finishIndex, BigOStack *stack, int key, char *functionName, int recursiveException)
{
    int *newStart = (int *)malloc(sizeof(int *));
    newStart[0] = startIndex;
    int *newEnd = (int *)malloc(sizeof(int *));
    newEnd[0] = finishIndex;

    while (1)
    {
        int *newForStart = (int *)malloc(sizeof(int *));
        newForStart[0] = findStartOfFor(newStart[0], newEnd[0]);

        int *newWhileStart = (int *)malloc(sizeof(int *));
        newWhileStart[0] = findStartOfWhile(newStart[0], newEnd[0]);

        int *newDoWhileStart = (int *)malloc(sizeof(int *));
        newDoWhileStart[0] = findStartOfDoWhile(newStart[0], newEnd[0]);

        int *newFunctionStart = (int *)malloc(sizeof(int *));
        newFunctionStart[0] = findStartOfLocalFunction(newStart[0], newEnd[0], recursiveException);

        if (newForStart[0] == INT_MAX && newWhileStart[0] == INT_MAX && newFunctionStart[0] == INT_MAX && newDoWhileStart[0] == INT_MAX)
        {
            break;
        }

        int *newEnd = (int *)malloc(sizeof(int *));
        int *newSender = (int *)malloc(sizeof(int *));

        if (newForStart[0] < newWhileStart[0] &&
            newForStart[0] < newFunctionStart[0] &&
            newForStart[0] < newDoWhileStart[0])
        {
            // Detected a For Loop
            newEnd[0] = findFinishOfLoop(newForStart[0]);

            newSender[0] = checkPieceOfFor(newForStart[0], newEnd[0]);
            BigOStack *newBigO = (BigOStack *)malloc(sizeof(BigOStack *));
            newBigO = pushBigO(newSender[0], stack);

            checkForBigO(newForStart[0] + 2, newEnd[0], newBigO, key, functionName, recursiveException);
        }
        else if (newWhileStart[0] < newForStart[0] &&
                 newWhileStart[0] < newFunctionStart[0] &&
                 newWhileStart[0] < newDoWhileStart[0])
        {
            // Detected a While Loop
            newEnd[0] = findFinishOfLoop(newWhileStart[0]);

            newSender[0] = checkPieceOfWhile(newWhileStart[0], newEnd[0], -1, -1);
            BigOStack *newBigO = (BigOStack *)malloc(sizeof(BigOStack *));
            newBigO = pushBigO(newSender[0], stack);

            checkForBigO(newWhileStart[0] + 2, newEnd[0], newBigO, key, functionName, recursiveException);
        }
        else if (newDoWhileStart[0] < newWhileStart[0] &&
                 newDoWhileStart[0] < newFunctionStart[0] &&
                 newDoWhileStart[0] < newForStart[0])
        {
            int finishOfDo = findFinishOfLoop(newDoWhileStart[0]);
            newEnd[0] = findFinishOfDoWhile(finishOfDo);

            newSender[0] = checkPieceOfWhile(newWhileStart[0], newEnd[0], newDoWhileStart[0], finishOfDo);
            BigOStack *newBigO = (BigOStack *)malloc(sizeof(BigOStack *));
            newBigO = pushBigO(newSender[0], stack);

            checkForBigO(newWhileStart[0] + 2, newEnd[0], newBigO, key, functionName, recursiveException);
        }
        else
        {
            // Detected a Function
            newEnd[0] = localFunctionEndIndex(newFunctionStart[0]);

            char *nameOfTheFunction = (char *)malloc(sizeof(char) * (newEnd[0] - newFunctionStart[0] - 1));

            for (int a = newFunctionStart[0]; a <= newEnd[0] - 2; a++)
            {
                nameOfTheFunction[a - newFunctionStart[0]] = codeArray[a];
                if (a == newEnd[0] - 2 || codeArray[a] == '(')
                {
                    nameOfTheFunction[a - newFunctionStart[0]] = '\0';
                }
            }

            // printf("%s=%ld %s=%ld", nameOfTheFunction, strlen(nameOfTheFunction), functionName, strlen(functionName));
            //  CHECK IF RECURSIVE
            if (stringCompareByLast(functionName, nameOfTheFunction))
            {
                // RECURSIVE

                newSender[0] = findBigOByMark(newFunctionStart[0] + strlen(nameOfTheFunction), newEnd[0]);
                BigOStack *newBigO = (BigOStack *)malloc(sizeof(BigOStack *));
                newBigO = pushBigO(newSender[0], stack);

                checkForBigO(startIndex, finishIndex, newBigO, key, functionName, newFunctionStart[0]);
            }
            else
            {
                // NOT RECURSIVE
                int *startFunctionIndex = (int *)malloc(sizeof(int *));
                int *endFunctionIndex = (int *)malloc(sizeof(int *));
                int *newKey = (int *)malloc(sizeof(int *));
                BigOStack *newStack = (BigOStack *)malloc(sizeof(BigOStack *));

                startFunctionIndex[0] = findStartOfGlobalFunction(nameOfTheFunction);
                endFunctionIndex[0] = findEndOfGlobalFunction(startFunctionIndex[0]);
                newKey[0] = takeNewKey();

                checkForBigO(startFunctionIndex[0], endFunctionIndex[0], stack, newKey[0], nameOfTheFunction, -1);

                newStack = calculateBigOViaKey(newKey[0]);

                // concatStacks(newStack, stack);

                pushStackToMainContainer(newStack, key);
            }
        }

        newStart[0] = newEnd[0];
    }

    pushStackToMainContainer(stack, key);
}

int calculateFunctionBigOKey(int startIndex)
{
    int colonCounter = 0;

    int firstIndex = -1;
    int lastIndex = -1;

    int functionNameStart = -1;
    int functionNameEnd = -1;

    for (int a = startIndex; a < codeArrayLength; a++)
    {
        if (functionNameStart == -1 && isalpha(codeArray[a]))
        {
            functionNameStart = a;
        }
        if (functionNameEnd == -1 && codeArray[a] == '(')
        {
            functionNameEnd = a - 1;
        }

        if (codeArray[a] == '{')
        {
            colonCounter += 1;
            if (firstIndex == -1)
            {
                firstIndex = a;
            }
        }
        else if (codeArray[a] == '}')
        {
            colonCounter -= 1;
            if (colonCounter == 0)
            {
                lastIndex = a;
                break;
            }
        }
    }

    int key = takeNewKey();

    char *nameOfTheFunction = (char *)malloc(sizeof(char) * (functionNameEnd - functionNameStart + 2));

    for (int a = functionNameStart; a <= functionNameEnd + 1; a++)
    {
        nameOfTheFunction[a - functionNameStart] = codeArray[a];
        if (a == functionNameEnd + 1)
        {
            nameOfTheFunction[a - functionNameStart] = '\0';
        }
    }

    checkForBigO(firstIndex, lastIndex, NULL, key, nameOfTheFunction, -1);

    return key;
}

void printUpperNumber(int number)
{
    char *test[10] = {"⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹"};
    int temp = number;
    int reversed = 0;

    while (temp > 0)
    {
        reversed = reversed * 10 + temp % 10;
        temp = temp / 10;
    }

    while (reversed > 0)
    {
        printf("%s", test[reversed % 10]);
        reversed = reversed / 10;
    }
}

void printDownerNumber(int number)
{
    char *test[10] = {"₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉"};
    int temp = number;
    int reversed = 0;

    while (temp > 0)
    {
        reversed = reversed * 10 + temp % 10;
        temp = temp / 10;
    }

    while (reversed > 0)
    {
        printf("%s", test[reversed % 10]);
        reversed = reversed / 10;
    }
}

void printBigOStack(BigOStack *stack)
{
    int printedBefore = 0;
    int nCounter = 0;

    BigOStack *stackPointer = stack;
    while (stackPointer != NULL)
    {
        if (stackPointer->bigO == 0)
        {
            nCounter += 1;
        }
        stackPointer = stackPointer->prev;
    }

    if (nCounter > 0)
    {
        printf("n");
        if (nCounter > 1)
        {
            printUpperNumber(nCounter);
        }
        printedBefore = 1;
    }

    stackPointer = stack;

    while (stackPointer != NULL)
    {
        if (stackPointer->bigO != 0)
        {
            if (printedBefore)
            {
                printf(".log");
            }
            else
            {
                printf("log");
                printedBefore = 0;
            }

            printDownerNumber(stackPointer->bigO);
            printf("(n)");
        }
        stackPointer = stackPointer->prev;
    }
}

void printContainerBigOViaKey(int key)
{
    int lengthOfMainCon = lengthOfMainContainer();

    int *nBigO = (int *)malloc(sizeof(int) * lengthOfMainCon);
    int *logBigO = (int *)malloc(sizeof(int) * lengthOfMainCon);

    BigOContainer *containerStack = mainContainer;

    int maxIndex = 0;

    for (int a = 0; a < lengthOfMainCon; a++)
    {
        if (containerStack->key != key)
        {
            containerStack = containerStack->next;
            continue;
        }

        BigOStack *stackPointer = containerStack->stack;
        int counterBigO = 0;
        int multiplyOfLogs = 1;

        while (stackPointer != NULL)
        {
            if (stackPointer->bigO == 0)
            {
                counterBigO += 1;
            }
            else
            {
                multiplyOfLogs *= stackPointer->bigO;
            }

            stackPointer = stackPointer->prev;
        }

        nBigO[maxIndex] = counterBigO;
        logBigO[maxIndex] = multiplyOfLogs;
        containerStack = containerStack->next;
        maxIndex += 1;
    }

    int maxN = -1;
    int maxLog = 0;
    int currentIndex = -1;

    for (int a = 0; a < maxIndex; a++)
    {
        if (nBigO[a] > maxN)
        {
            maxN = nBigO[a];
            maxLog = logBigO[a];
            currentIndex = a;
        }
        else if (nBigO[a] == maxN && logBigO[a] > maxLog)
        {
            maxN = nBigO[a];
            maxLog = logBigO[a];
            currentIndex = a;
        }
    }

    // Printing n

    if (maxN > 0)
    {
        printf("\nn");
        if (maxN > 1)
        {
            printUpperNumber(maxN);
        }
    }

    // Printing logarithms
    containerStack = mainContainer;

    for (int a = 0; a < currentIndex; a++)
    {
        if (containerStack->key != key)
        {
            a -= 1;
        }
        containerStack = containerStack->next;
    }

    BigOStack *stackPointer = containerStack->stack;

    while (stackPointer != NULL)
    {
        if (stackPointer->bigO != 0)
        {
            printf(".log");
            printDownerNumber(stackPointer->bigO);
            printf("(n)");
        }

        stackPointer = stackPointer->prev;
    }
}
// #endregion
int minNumber(int a, int b)
{
    if (b < a)
    {
        return b;
    }
    return a;
}

int maxNumber(int a, int b)
{
    if (b < a)
    {
        return a;
    }
    return b;
}

int checkStringByStarts(char *dizi1, char *dizi2)
{
    for (int a = 0; a < minNumber(strlen(dizi1), strlen(dizi2)); a++)
    {
        if (dizi1[a] != dizi2[a])
        {
            return 0;
        }
    }
    return 1;
}

int checkSizeOfVarByStart(int startIndex)
{
    int limitNumber = 7;
    if (codeArrayLength - startIndex < 7)
    {
        limitNumber = codeArrayLength - startIndex;
    }
    char *word = (char *)malloc(sizeof(char) * (limitNumber));
    for (int a = startIndex; a < startIndex + limitNumber; a++)
    {
        word[a - startIndex] = codeArray[a];
    }
    word[limitNumber] = '\0';

    if (checkStringByStarts("int", word))
    {
        return sizeof(int);
    }
    else if (checkStringByStarts("char", word))
    {
        return sizeof(char);
    }
    else if (checkStringByStarts("short", word))
    {
        return sizeof(short);
    }
    else if (checkStringByStarts("long", word))
    {
        return sizeof(long);
    }
    else if (checkStringByStarts("double", word))
    {
        return sizeof(double);
    }
    else if (checkStringByStarts("float", word))
    {
        return sizeof(float);
    }
    else
    {
        return -1;
    }
}

int checkSizeOfVarByEnd(int startIndex)
{
    int limitNumber = 7;
    if (codeArrayLength - startIndex < 7)
    {
        limitNumber = codeArrayLength - startIndex;
    }
    char *word = (char *)malloc(sizeof(char) * (limitNumber));
    for (int a = startIndex; a < startIndex + limitNumber; a++)
    {
        word[a - startIndex] = codeArray[maxNumber(a, 0)];
    }
    word[limitNumber] = '\0';

    if (stringCompareByLast(word, "int"))
    {
        return sizeof(int);
    }
    else if (stringCompareByLast(word, "char"))
    {
        return sizeof(char);
    }
    else if (stringCompareByLast(word, "short"))
    {
        return sizeof(short);
    }
    else if (stringCompareByLast(word, "long"))
    {
        return sizeof(long);
    }
    else if (stringCompareByLast(word, "double"))
    {
        return sizeof(double);
    }
    else if (stringCompareByLast(word, "float"))
    {
        return sizeof(float);
    }
    else
    {
        return -1;
    }
}

void variableCalculator(int startIndex, int endIndex, int frontNumber)
{
    int upperNumber = 0;
    for (int a = startIndex; a <= endIndex; a++)
    {
        if (codeArray[a] == '[')
        {
            upperNumber += 1;
        }
    }

    if (upperNumber == 0)
    {
        BigOStack *newStack = (BigOStack *)malloc(sizeof(BigOStack));
        newStack->bigO = frontNumber;
        newStack->prev = diskStack;
        diskStack = newStack;
        return;
    }
    else
    {
        BigOStack *stackPointer = diskStack;
        for (int a = 0; a < frontNumber; a++)
        {
            BigOStack *newStack = (BigOStack *)malloc(sizeof(BigOStack));
            newStack->bigO = -1 * upperNumber;
            newStack->prev = stackPointer;
            stackPointer = newStack;
        }

        diskStack = stackPointer;
        return;
    }
}

int findNearestIndexOfVar(int startIndex, int endIndex)
{
    for (int a = startIndex; a <= endIndex; a++)
    {
        int varIndex = checkSizeOfVarByStart(a);
        if (varIndex == -1)
        {
            continue;
        }
        return a;
    }
    return -1;
}

void calculateDiskCompOfFunction(int startIndex, char *nameOfFunc)
{
    int bracketCounter = 0;
    int startOfParam = -1;
    int endOfParam = -1;

    int endOfFunc = findEndOfGlobalFunction(startIndex);
    int startOfFunction = -1;
    for (int a = startIndex; a <= endOfFunc; a++)
    {
        if (codeArray[a] == '(')
        {
            if (startOfParam == -1)
            {
                startOfParam = a + 1;
            }
            bracketCounter += 1;
        }
        else if (codeArray[a] == ')')
        {
            bracketCounter -= 1;
            if (bracketCounter == 0)
            {
                startOfFunction = a + 1;
                endOfParam = a - 1;
                break;
            }
        }
    }
    int newStart = startOfParam;
    int newEnd = -1;
    for (int a = startOfParam; a <= endOfParam; a++)
    {
        int frontNumber = checkSizeOfVarByStart(newStart);
        if (codeArray[a] == ',')
        {
            newEnd = a - 1;
            variableCalculator(newStart, newEnd, frontNumber);
            newStart = a + 1;
        }
        else if (a == endOfParam)
        {
            newEnd = a;
            variableCalculator(newStart, newEnd, frontNumber);
        }
    }

    int currentIndex = startOfFunction;
    int whileDoor = 1;
    while (whileDoor)
    {
        int varIndex = findNearestIndexOfVar(currentIndex, endOfFunc);
        if (varIndex == -1)
        {
            break;
        }
        int frontNumber = checkSizeOfVarByStart(varIndex);
        int cursorIndex = varIndex;
        for (int a = varIndex; a <= endOfFunc; a++)
        {
            if (codeArray[a] == ';')
            {
                variableCalculator(cursorIndex, a - 1, frontNumber);
                currentIndex = a;
                break;
            }
            if (codeArray[a] == ',')
            {
                variableCalculator(cursorIndex, a - 1, frontNumber);
                cursorIndex = a + 1;
            }
        }
    }
    int tempStart = startOfFunction;
    while (1)
    {
        int startOfLocal = findStartOfLocalFunction(tempStart, endOfFunc, -1);
        if (startOfLocal == INT_MAX)
        {
            break;
        }

        int endOfLocal = localFunctionEndIndex(startOfLocal);
        char *funcName = (char *)malloc(sizeof(char) * endOfLocal - startOfLocal);
        for (int a = startOfLocal; a <= endOfLocal; a++)
        {
            if (codeArray[a] == '(')
            {
                funcName[a - startOfLocal] = '\0';
                break;
            }
            funcName[a - startOfLocal] = codeArray[a];
        }
        int startOfGlobal = findStartOfGlobalFunction(funcName);
        if (stringCompareByLast(funcName, nameOfFunc))
        {
            // recursive
            int frontNumber = checkSizeOfVarByEnd(startOfGlobal - 7);
            if (frontNumber == -1)
            {
                printf("HATA KOD:465");
                exit(1);
            }

            int startOfParam = -1;
            for (int a = startOfLocal; codeArray[a] != '('; a++)
            {
                startOfParam = a + 2;
            }

            int bigO = findBigOByMark(startOfParam, endOfLocal - 2);

            for (int a = 0; a < frontNumber; a++)
            {
                if (bigO == 0)
                {
                    BigOStack *stackPointer = pushBigO(-1, diskStack);
                    diskStack = stackPointer;
                }
                else
                {
                    BigOStack *logStackPointer = pushBigO(bigO, diskLogStack);
                    diskLogStack = logStackPointer;
                }
            }
        }
        else
        {
            // not recursive
            calculateDiskCompOfFunction(startOfGlobal, funcName);
        }

        tempStart = endOfLocal;
    };

    for (int a = startOfFunction; a <= endOfFunc; a++)
    {
        if (codeArray[a] == 'r' &&
            codeArray[a + 1] == 'e' &&
            codeArray[a + 2] == 't' &&
            codeArray[a + 3] == 'u' &&
            codeArray[a + 4] == 'r' &&
            codeArray[a + 5] == 'n')
        {
            // found return

            for (int u = 0; u < strlen(nameOfFunc); u++)
            {
                if (nameOfFunc[u] == codeArray[a + 6 + u])
                {
                    return;
                }
            }

            for (int e = a + 6; codeArray[e] != ';'; e++)
            {
                if (isalpha(codeArray[e]))
                {
                    int globalIndex = findStartOfGlobalFunction(nameOfFunc);
                    int newIndex = -1;
                    for (int i = globalIndex;
                         (i == 0) ||
                         (codeArray[i] != '}' &&
                          codeArray[i] != '>' &&
                          codeArray[i] != ';');
                         i--)
                    {
                        newIndex = i;
                    }

                    int bigO = checkSizeOfVarByStart(newIndex);

                    BigOStack *stackPointer = pushBigO(bigO, diskStack);
                    diskStack = stackPointer;

                    break;
                }
            }
        }
    }
}

void printDiskBigO()
{
    int stackCounter = 0;
    BigOStack *stackPointer = diskStack;
    BigOStack *logStackPointer = diskLogStack;
    int baseNumber = 0;
    int stackSize = 0;
    while (stackPointer != NULL)
    {
        if (stackPointer->bigO > 0)
        {
            baseNumber += stackPointer->bigO;
            stackCounter += 1;
        }
        stackPointer = stackPointer->prev;
        stackSize += 1;
    }
    printf("%d", baseNumber);
    int upperNumber = -1;
    while (stackCounter < stackSize)
    {
        int frontNumber = 0;
        stackPointer = diskStack;
        while (stackPointer != NULL)
        {
            if (stackPointer->bigO == upperNumber)
            {
                frontNumber += 1;
                stackCounter += 1;
            }
            stackPointer = stackPointer->prev;
        }

        if (frontNumber != 0)
        {
            printf("+");
            if (frontNumber > 1)
            {
                printf("%d", frontNumber);
            }
            printf("n");
            if (upperNumber < -1)
            {
                printUpperNumber(upperNumber * -1);
            }
        }

        upperNumber -= 1;
    }
    int logStackSize = 0;
    int logStackCounter = 0;
    while (logStackPointer != NULL)
    {
        logStackPointer = logStackPointer->prev;
        logStackSize += 1;
    }
    int logNumber = 2;
    while (logStackCounter < logStackSize)
    {
        int frontNumber = 0;
        logStackPointer = diskLogStack;

        while (logStackPointer != NULL)
        {
            if (logStackPointer->bigO == logNumber)
            {
                frontNumber += 1;
                logStackCounter += 1;
            }
            logStackPointer = logStackPointer->prev;
        }

        if (frontNumber != 0)
        {
            printf("+");
            if (frontNumber > 1)
            {
                printf("%d", frontNumber);
            }
            printf("log");
            printDownerNumber(logNumber);
            printf("(n)");
        }

        logNumber += 1;
    }
}

int main()
{
    codeArray = takeCodeToArray();

    if (checkForBrackets())
    {
        printf("\nKod uygun.\n\nZaman karmaşıklığı: ");

        int mainIndex = findStartOfGlobalFunction("intmain");
        if (mainIndex == INT_MAX)
        {
            mainIndex = findStartOfGlobalFunction("voidmain");
        }
        if (mainIndex == INT_MAX)
        {
            mainIndex = 0;
        }

        int key = calculateFunctionBigOKey(mainIndex);
        BigOStack *stack = calculateBigOViaKey(key);
        printBigOStack(stack);

        int wordLength = 0;

        for (int a = mainIndex; codeArray[a] != '('; a++)
        {
            wordLength += 1;
        }

        char *word = (char *)malloc(sizeof(char) * (wordLength + 1));

        for (int a = mainIndex; codeArray[a] != '('; a++)
        {
            word[a - mainIndex] = codeArray[a];
        }

        word[wordLength] = '\0';

        printf("\n\nYer kamaşıklığı: ");

        calculateDiskCompOfFunction(mainIndex, word);
        printDiskBigO(diskStack, diskLogStack);

        printf("\n\n");
    }
    else
    {
        printf("\nKod uygun değil.\n");
    }
}