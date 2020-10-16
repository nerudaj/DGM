#pragma once

#include <map>
#include <vector>
#include <stdexcept>
#include <functional>
#include <string>

namespace dgm {
	class ShellModule {
	public:
		using Arguments = std::vector<std::string>;

	protected:
		struct Action {
			unsigned argc; /// Number of arguments required by action
			std::string description; /// Description of action for help dialog
			std::function<std::string(const Arguments&)> callback; /// Callback to execute. Returns stdout of the action
		};

		/**
		 *  \brief Add action to a module
		 *
		 *  Use this method to add unique actions to a particular module.
		 *  This method checks uniqueness of each action name.
		 *  "help" is always present by default.
		 */
		void addAction(const std::string &name, const Action& action);

	private:
		std::map<std::string, Action> actions;
		std::string name;

		std::string printHelp() const;

	public:
		/**
		 *  \brief Execute action on module with arguments
		 *
		 *  \details Returns stdout of the command, stderr is returned via exception.
		 *  Catch ShellException to read the stderr.
		 *
		 *  Arguments will be stripped of the action name before passing
		 *  them to particular action.
		 */
		std::string execute(const Arguments& args);

		const std::string& getName() const {
			return name;
		}

		virtual std::string getDescription() const = 0;

		ShellModule(const std::string &name);
		virtual ~ShellModule() {}
	};

	class ShellModuleVars : public ShellModule {
	public:
		class Variable {
		public:
			virtual void set(const std::string& value) = 0;
			virtual std::string getType() const = 0;
			virtual std::string getValue() const = 0;
		};

	private:
		std::map<std::string, Variable*> variables;

		std::string formatVariable(const std::string& name) const;
	public:
		template<typename T>
		void bindVariable(const std::string& name, T* ptr);

		virtual std::string getDescription() const override;

		ShellModuleVars();
		~ShellModuleVars();
	};

	class Shell {
	private:
		std::map<std::string, ShellModule*> modules;

		std::string printGlobalHelp() const;

		RingBuffer<std::string, 10> history;

	public:
		/**
		 *  \brief Executes command in the shell
		 *
		 *  \return Returns stdout of the command. If command fails,
		 *  it throws a ShellException.
		 */
		std::string execute(const std::string &command);

		void install(ShellModule& module);

		const RingBuffer<std::string, 10>& getHistory() const {
			return history;
		}

		Shell();
		Shell(Shell &&other) = delete;
		Shell(const Shell &other) = delete;
		Shell &operator=(const Shell &other) = delete;
		~Shell();
	};
}