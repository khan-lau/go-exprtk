#ifndef EXPRTKWRAPPER_HPP
#define EXPRTKWRAPPER_HPP

#ifdef __cplusplus
extern "C" {
#endif

typedef void* exprtkWrapper;

exprtkWrapper exprtkWrapperInit(void);

void setExpressionString(exprtkWrapper, char* expr);

const char* getExpressionString(exprtkWrapper);

void addDoubleVariable(exprtkWrapper, char* var,double val);

void addStringVariable(exprtkWrapper, char* var,char* val);

void addVectorVariable(exprtkWrapper, char* var,double val[],int len);

int compileExpression(exprtkWrapper);

void setDoubleVariableValue(exprtkWrapper, char* var, double val);

void setStringVariableValue(exprtkWrapper, char* var, char* val);

void setVectorVariableValue(exprtkWrapper, char* var, double val[], int len);

double getEvaluatedValue(exprtkWrapper);
const char* printParseErr(exprtkWrapper);


const char* version();

//类型定义
typedef double (*fun_double)();
typedef double (*fun1_double)(double);
typedef double (*fun2_double)(double,double);
typedef double (*fun3_double)(double,double,double);
typedef double (*fun4_double)(double,double,double,double);
typedef double (*fun5_double)(double,double,double,double,double);
typedef double (*fun6_double)(double,double,double,double,double,double);

void freeExprtk(exprtkWrapper obj);
void addFunction(exprtkWrapper obj, char* funStr,fun_double fun);
void addFunction1(exprtkWrapper obj, char* funStr,fun1_double fun);
void addFunction2(exprtkWrapper obj, char* funStr,fun2_double fun);
void addFunction3(exprtkWrapper obj, char* funStr,fun3_double fun);
void addFunction4(exprtkWrapper obj, char* funStr,fun4_double fun);
void addFunction5(exprtkWrapper obj, char* funStr,fun5_double fun);
void addFunction6(exprtkWrapper obj, char* funStr,fun6_double fun);

//自定义函数
int isUserFunction(exprtkWrapper obj, char* funStr);

double hoursInYear();
double hoursInMonth();




#ifdef __cplusplus
}
#endif

#endif // EXPRTKWRAPPER_HPP
