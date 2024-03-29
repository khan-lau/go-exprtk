#include "exprtkwrapper.h"
#include "exprtk.hpp"
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <type_traits>
// #include <time.h>

struct DoubleValue{
    std::string varName;
    double value;
};

struct StringValue{
    std::string varName;
    std::string value;
};

struct VectorValue{
    std::string varName;
    std::vector<double> value;
};


// 自定义函数 总和 sum(array), 接受任意参数 by khan.lau 2024-01-28
// template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
template <typename T>
struct SumFunction : public exprtk::igeneric_function<T> {
    SumFunction() : exprtk::igeneric_function<T>("V") {}
    
    typedef typename exprtk::igeneric_function<T>::parameter_list_t parameter_list_t;
    typedef typename exprtk::igeneric_function<T>::generic_type     generic_type;
    typedef typename generic_type::scalar_view                      scalar_t;
    typedef typename generic_type::vector_view                      vector_t;

    inline T operator() (parameter_list_t params) {
        if (params.size() < 1) {
            return (T) 0;
        }
        
        vector_t vec = vector_t(params[0]);
        return std::accumulate(vec.begin(), vec.end(), 0.0);
    }
};

// 自定义函数 平均值 avg(array), 接受任意参数 by khan.lau 2024-01-28
// template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
template <typename T>
struct AvgFunction : public exprtk::igeneric_function<T> {
    AvgFunction() : exprtk::igeneric_function<T>("V") {}
    
    typedef typename exprtk::igeneric_function<T>::parameter_list_t parameter_list_t;
    typedef typename exprtk::igeneric_function<T>::generic_type     generic_type;
    typedef typename generic_type::scalar_view                      scalar_t;
    typedef typename generic_type::vector_view                      vector_t;

    inline T operator() (parameter_list_t params) {
        if (params.size() < 1) {
            return (T) 0;
        }
        vector_t vec = vector_t(params[0]);
        return std::accumulate(vec.begin(), vec.end(), 0.0) / (T)vec.size();
    }
};

// 自定义函数 max(array), 接受任意参数 by khan.lau 2024-01-28
template <typename T>
struct MaxFunction : public exprtk::igeneric_function<T> {
    MaxFunction() : exprtk::igeneric_function<T>("V") {}
    
    typedef typename exprtk::igeneric_function<T>::parameter_list_t parameter_list_t;
    typedef typename exprtk::igeneric_function<T>::generic_type     generic_type;
    typedef typename generic_type::scalar_view                      scalar_t;
    typedef typename generic_type::vector_view                      vector_t;

    inline T operator() (parameter_list_t params) {
        if (params.size() < 1) {
            return (T) 0;
        }
        vector_t vec = vector_t(params[0]);

        auto iter = std::max_element(vec.begin(), vec.end());
        return *iter;
    }
};

// 自定义函数 min(array), 接受任意参数 by khan.lau 2024-01-28
template <typename T>
struct MinFunction : public exprtk::igeneric_function<T> {
    MinFunction() : exprtk::igeneric_function<T>("V") {}
    
    typedef typename exprtk::igeneric_function<T>::parameter_list_t parameter_list_t;
    typedef typename exprtk::igeneric_function<T>::generic_type     generic_type;
    typedef typename generic_type::scalar_view                      scalar_t;
    typedef typename generic_type::vector_view                      vector_t;

    inline T operator() (parameter_list_t params) {
        if (params.size() < 1) {
            return (T) 0;
        }
        vector_t vec = vector_t(params[0]);

        auto iter = std::min_element(vec.begin(), vec.end());
        return *iter;
    }
};

// 自定义函数 first(array), 接受任意参数 by khan.lau 2024-01-28
template <typename T>
struct FirstFunction : public exprtk::igeneric_function<T> {
    FirstFunction() : exprtk::igeneric_function<T>("V") {}
    
    typedef typename exprtk::igeneric_function<T>::parameter_list_t parameter_list_t;
    typedef typename exprtk::igeneric_function<T>::generic_type     generic_type;
    typedef typename generic_type::scalar_view                      scalar_t;
    typedef typename generic_type::vector_view                      vector_t;

    inline T operator() (parameter_list_t params) {
        if (params.size() < 1) {
            return (T) 0;
        }
        vector_t vec = vector_t(params[0]);
        std::size_t n = vec.size();
        if (n < 1) {
            return (T)0;
        }
        return vec[0];
    }
};

// 自定义函数 last(array), 接受任意参数 by khan.lau 2024-01-28
template <typename T>
struct LastFunction : public exprtk::igeneric_function<T> {
    LastFunction() : exprtk::igeneric_function<T>("V") {}
    
    typedef typename exprtk::igeneric_function<T>::parameter_list_t parameter_list_t;
    typedef typename exprtk::igeneric_function<T>::generic_type     generic_type;
    typedef typename generic_type::scalar_view                      scalar_t;
    typedef typename generic_type::vector_view                      vector_t;

    inline T operator() (parameter_list_t params) {
        if (params.size() < 1) {
            return (T) 0;
        }
        vector_t vec = vector_t(params[0]);
        std::size_t n = vec.size();
        if (n < 1) {
            return (T)0;
        }

        return vec[n-1];
    }
};

// 自定义函数 离散率 dev(array), 实际是要求标准差, 接受任意参数 by khan.lau 2024-01-28
template <typename T>
struct DevFunction : public exprtk::igeneric_function<T> {
    DevFunction() : exprtk::igeneric_function<T>("V") {}
    
    typedef typename exprtk::igeneric_function<T>::parameter_list_t parameter_list_t;
    typedef typename exprtk::igeneric_function<T>::generic_type     generic_type;
    typedef typename generic_type::scalar_view                      scalar_t;
    typedef typename generic_type::vector_view                      vector_t;

    inline T operator() (parameter_list_t params) {
        if (params.size() < 1) {
            return (T) 0;
        }
        vector_t vec = vector_t(params[0]);

        double mean = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();// 计算平均值

        // 计算方差
        double variance = 0.0;
        for (int i = 0; i < vec.size(); i++) {
            variance += pow(vec[i] - mean, 2);
        }
        variance /= vec.size();

        double standard_deviation = std::sqrt((double)variance);// 计算标准差
        // double discrete_rate = standard_deviation / mean;// 计算离散率

        return standard_deviation;
    }
};


// 自定义函数 差值 Diff(array), 接受任意参数 by khan.lau 2024-01-28
template <typename T>
struct DiffFunction : public exprtk::igeneric_function<T> {
    DiffFunction() : exprtk::igeneric_function<T>("V") {}
    
    typedef typename exprtk::igeneric_function<T>::parameter_list_t parameter_list_t;
    typedef typename exprtk::igeneric_function<T>::generic_type     generic_type;
    typedef typename generic_type::scalar_view                      scalar_t;
    typedef typename generic_type::vector_view                      vector_t;

    inline T operator() (parameter_list_t params) {
        if (params.size() < 1) {
            return (T) 0;
        }

        vector_t vec = vector_t(params[0]);
        std::size_t n = vec.size();
        if (n < 2) {
            return (T)0;
        } 

        return vec[n-1] - vec[0];
    }
};

// 自定义函数 加权平均数 wavg(array1, array2), 接受任意参数 by khan.lau 2024-01-29
template <typename T>
struct WavgFunction : public exprtk::igeneric_function<T> {
    WavgFunction() : exprtk::igeneric_function<T>("V") {}
    
    typedef typename exprtk::igeneric_function<T>::parameter_list_t parameter_list_t;
    typedef typename exprtk::igeneric_function<T>::generic_type     generic_type;
    typedef typename generic_type::scalar_view                      scalar_t;
    typedef typename generic_type::vector_view                      vector_t;

    inline T operator() (parameter_list_t params) {
        if (params.size() < 2) {
            return (T) 0;
        }
        vector_t values  = vector_t(params[0]);
        vector_t weights = vector_t(params[1]);
        std::size_t vn = values.size();
        std::size_t wn = weights.size();

        if ( vn != wn ) {
            return (T)0;
        }

        T sum = 0.0;
        T weight_sum = 0.0;
        for (std::size_t i = 0; i < values.size(); ++i) {
            sum += values[i] * weights[i];
            weight_sum += weights[i];
        }
        return sum / weight_sum;
    }
};



struct  ExprtkStruct
{
    std::string exprString;
    std::string errString;
    std::vector<DoubleValue*> doubleValue;
    std::vector<StringValue*> stringValue;
    std::vector<VectorValue*> vectorValue;

    exprtk::symbol_table<double> symbolTable;
    exprtk::expression<double> expression;
    exprtk::parser<double> parser;

    SumFunction<double>   sum_func;     // √ SUM(array)   计算数组值的总和
    AvgFunction<double>   avg_func;     // √ AVG(array)   计算数组值的平均值

    MaxFunction<double>   max_func;     // √ MAX(array)   找出数组中的最大值
    MinFunction<double>   min_func;     // √ MIN(array)   找出数组中的最小值

    FirstFunction<double> first_func;   // √ LAST(array)  获取数组中的最后一个值
    LastFunction<double>  last_func;    // √ FIRST(array) 获取数组中的第一个值

    DevFunction<double>   dev_func;     // √ DEV(array)   计算数组中值的离散率       - 标准差
    DiffFunction<double>  diff_func;    // √ DIFF(array)  计算数组连续数值之间的差值 - (last - first)

    WavgFunction<double>  wavg_func;    // √ WAVG(array1, array2) 计算加权平均数

    ExprtkStruct() {
        symbolTable.add_function("hours_in_month", hoursInMonth);
        symbolTable.add_function("hours_in_year", hoursInYear);

        symbolTable.add_function("x_sum",   sum_func);
        symbolTable.add_function("x_avg",   avg_func);
        symbolTable.add_function("x_min",   min_func);
        symbolTable.add_function("x_max",   max_func);
        symbolTable.add_function("x_first", first_func);
        symbolTable.add_function("x_last",  last_func);
        symbolTable.add_function("x_dev",   dev_func);
        symbolTable.add_function("x_diff",  diff_func);
        symbolTable.add_function("x_wavg",  wavg_func);
    }

    ~ExprtkStruct(){
        for (int i = 0; i < doubleValue.size();i++){
            delete doubleValue[i];
            doubleValue[i] = nullptr;
        }
        for (int i = 0; i < stringValue.size();i++){
            delete stringValue[i];
            stringValue[i] = nullptr;
        }
        for (int i = 0; i < vectorValue.size();i++){
            delete vectorValue[i];
            vectorValue[i] = nullptr;
        }

        /*doubleValue.clear();
        stringValue.clear();
        vectorValue.clear();
        symbolTable.clear();*/
    }
};

exprtkWrapper exprtkWrapperInit()
{
    ExprtkStruct* obj = new ExprtkStruct();
    return (void*)obj;
}

void freeExprtk(exprtkWrapper obj){
 	ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    delete exprtkStruct;
   // obj = nullptr;
}

void setExpressionString(exprtkWrapper obj, char* expr)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    std::string str = std::string(expr);
    exprtkStruct->exprString = str;
}

const char* getExpressionString(exprtkWrapper obj)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    return exprtkStruct->exprString.c_str();
}

int compileExpression(exprtkWrapper obj)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    exprtkStruct->symbolTable.add_constants();
    exprtkStruct->expression.register_symbol_table(exprtkStruct->symbolTable);
    bool compiled = exprtkStruct->parser.compile(exprtkStruct->exprString, exprtkStruct->expression);
    exprtkStruct->errString = exprtkStruct->parser.error();
    if (compiled){
        return 1;
    }else{
        return 0;
    }
}

void setDoubleVariableValue(exprtkWrapper obj,  char* var, double val)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    std::string varName = std::string(var);
    for (int i = 0; i < exprtkStruct->doubleValue.size();i++){
        if (varName.compare(exprtkStruct->doubleValue[i]->varName) == 0){
            exprtkStruct->doubleValue[i]->value = val;
            break;
        }
    }
}

void setStringVariableValue(exprtkWrapper obj,  char* var, char* val)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    std::string varName = std::string(var);
    std::string value = std::string(val);
    for (int i = 0; i < exprtkStruct->stringValue.size();i++){
        if (varName.compare(exprtkStruct->stringValue[i]->varName) == 0){
            exprtkStruct->stringValue[i]->value = value;
            break;
        }
    }
}

void setVectorVariableValue(exprtkWrapper obj,  char* var, double val[], int len)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    std::string varName = std::string(var);
    for (int i = 0; i < exprtkStruct->vectorValue.size();i++){
        if (varName.compare(exprtkStruct->vectorValue[i]->varName) == 0){
            exprtkStruct->vectorValue[i]->value.clear();
            for (int j = 0; j < len; j++){
                exprtkStruct->vectorValue[i]->value.push_back(double(val[j]));
            }
            break;
        }
    }
}

double getEvaluatedValue(exprtkWrapper obj)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    return exprtkStruct->expression.value();
}

void addDoubleVariable(exprtkWrapper obj, char* var,double val)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    DoubleValue* d = new(DoubleValue);
    d->varName = std::string(var);
    d->value = val;
    exprtkStruct->doubleValue.push_back(d);
    exprtkStruct->symbolTable.add_variable(d->varName,d->value);
    //sprintf("addDouble %s\n",d->varName);
}

void addStringVariable(exprtkWrapper obj, char* var,char* val)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    StringValue* s = new(StringValue);
    s->varName = std::string(var);
    s->value = std::string(val);
    exprtkStruct->stringValue.push_back(s);
    exprtkStruct->symbolTable.add_stringvar(s->varName,s->value);
}

void addVectorVariable(exprtkWrapper obj, char* var,double val[],int len)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    VectorValue* v = new(VectorValue);
    v->varName = std::string(var);
    for (int i = 0; i < len; i++){
        v->value.push_back(double(val[i]));
    }

    exprtkStruct->vectorValue.push_back(v);
    exprtkStruct->symbolTable.add_vector(v->varName,v->value);
}

const char* printParseErr(exprtkWrapper obj)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
    return exprtkStruct->errString.c_str();
}

void addFunction(exprtkWrapper obj,char* funStr,fun_double fun)
{
    ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
	std::string funcName = std::string(funStr);
	exprtkStruct->symbolTable.add_function(funcName, fun);
}
void addFunction1(exprtkWrapper obj, char* funStr,fun1_double fun)
{
	ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
	std::string funcName = std::string(funStr);
	exprtkStruct->symbolTable.add_function(funcName, fun);
}
void addFunction2(exprtkWrapper obj, char* funStr,fun2_double fun)
{
	ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
	std::string funcName = std::string(funStr);
	exprtkStruct->symbolTable.add_function(funcName, fun);
}
void addFunction3(exprtkWrapper obj, char* funStr,fun3_double fun)
{
	ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
	std::string funcName = std::string(funStr);
	exprtkStruct->symbolTable.add_function(funcName, fun);
}
void addFunction4(exprtkWrapper obj, char* funStr,fun4_double fun)
{
	ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
	std::string funcName = std::string(funStr);
	exprtkStruct->symbolTable.add_function(funcName, fun);
}
void addFunction5(exprtkWrapper obj, char* funStr,fun5_double fun)
{
	ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
	std::string funcName = std::string(funStr);
	exprtkStruct->symbolTable.add_function(funcName, fun);
}
void addFunction6(exprtkWrapper obj, char* funStr,fun6_double fun)
{
	ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
	std::string funcName = std::string(funStr);
	exprtkStruct->symbolTable.add_function(funcName, fun);
}

//是否是用户自定义函数
int isUserFunction(exprtkWrapper obj, char* funStr) {
	ExprtkStruct* exprtkStruct = (ExprtkStruct*)obj;
	std::string funcName = std::string(funStr);
	if (exprtkStruct->symbolTable.is_function(funcName)){
		return 1;
	}
	return 0;
}


const char* version(){
    static std::string ver = exprtk::information::data();
    return ver.c_str();
}

//自定义函数
//获取本年的小时数
double hoursInYear() {
	time_t nowTime = time(0);
	tm nTime, timeInfo = tm{};
    #if defined(_WIN32) || defined(_WIN64)
	gmtime_s(&nTime,&nowTime);
    #else
    tm * tmp = gmtime(&nowTime);
    nTime = *tmp;
    #endif
	timeInfo.tm_year = nTime.tm_year;
	timeInfo.tm_mday = 1;

	time_t oldTime = mktime(&timeInfo);
	double n = floor(difftime(nowTime, oldTime) / 3600);
	if (n < 1){
	    n = 1;
	}
	return n;
}

//获取本月的小时数
double hoursInMonth() {
	time_t nowTime = time(0);
	tm nTime, timeInfo = tm{};
    #if defined(_WIN32) || defined(_WIN64)
	gmtime_s(&nTime,&nowTime);
    #else
    tm * tmp = gmtime(&nowTime);
    nTime = *tmp;
    #endif
	timeInfo.tm_year = nTime.tm_year;
	timeInfo.tm_mon = nTime.tm_mon;
	timeInfo.tm_mday = 1;

	time_t oldTime = mktime(&timeInfo);
	double n = floor(difftime(nowTime, oldTime) / 3600);
	if (n < 1){
    	n = 1;
    }
    return n;
}


