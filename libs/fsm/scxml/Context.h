#pragma once
#ifndef FSM_CONTEXT_HEADER_
#define FSM_CONTEXT_HEADER_
#include <string>
#include <json/json.h>


namespace fsm{

	class Evaluator;
	class  Context
	{
	friend class Evaluator;
	public:
		Context(Evaluator * evl, Context * _parent):evaluator(evl),parent(_parent){
		};
		virtual ~Context(void){
		};

	public:
		///<summary>
		///对一个变量设置新值。
		///</summary>
		///<returns></returns>
		virtual void setVar(const std::string & name, const Json::Value & value) = 0;

		///<summary>
		///获取一个变量值。
		///</summary>
		///<returns>返回此变量值。</returns>
		virtual Json::Value getVar(const std::string &name) = 0;

		///<summary>
		///删除一个变量。
		///</summary>
		///<returns></returns>
		virtual void deleteVar(const std::string & name) = 0;

		/// <summary>
		/// Get the parent Context, may be null.
		/// </summary>
		/// <returns> The parent Context in a chained Context environment </returns>
		virtual Context *getParent() = 0;
		///<summary>
		///计算一段表达式脚本。
		///</summary>
		///<returns>返回此表达式执行的结果，转换为string类型。</returns>
		virtual Json::Value eval(const std::string &expr,const std::string &filename, unsigned int line) = 0;

		///<summary>
		///计算一段boolen表达式脚本
		///</summary>
		///<returns>返回此表达式执行的结果。</returns>
		virtual bool evalCond(const std::string &expr,const std::string &filename, unsigned int line) = 0;

		///<summary>
		///获取它的虚拟机。
		///</summary>
		///<returns>返回虚拟机指针。</returns>
		virtual Evaluator * getEvaluator(){ return evaluator; };

		///<summary>
		///执行一段脚本文件。
		///</summary>
		virtual void ExecuteFile(const std::string &fileName) = 0;

	protected:
		Evaluator * evaluator;
		//父Context
		Context *parent;
	};

}

#endif //end Context