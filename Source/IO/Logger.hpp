#pragma once

#include <format>
#include <memory>
#include <optional>
#include <source_location>
#include <string>
#include <vector>

#define LOG_INFO(...) conv::Logger::GetGlobalLogger().Info(__VA_ARGS__)
#define LOG_WARN(...) conv::Logger::GetGlobalLogger().Warn(__VA_ARGS__)
#define LOG_ERROR(...) conv::Logger::GetGlobalLogger().Error(__VA_ARGS__)
#define LOG_DEBUG(...) conv::Logger::GetGlobalLogger().Debug(__VA_ARGS__)
#define LOG_INFO_TRACE(...) conv::Logger::GetGlobalLogger().InfoTrace(std::source_location::current(), __VA_ARGS__)
#define LOG_WARN_TRACE(...) conv::Logger::GetGlobalLogger().WarnTrace(std::source_location::current(), __VA_ARGS__)
#define LOG_ERROR_TRACE(...) conv::Logger::GetGlobalLogger().ErrorTrace(std::source_location::current(), __VA_ARGS__)
#define LOG_DEBUG_TRACE(...) conv::Logger::GetGlobalLogger().DebugTrace(std::source_location::current(), __VA_ARGS__)

namespace conv
{
	class LoggerSink;
	
	class Logger
	{
	public:
		enum class Level
		{
			DEBUG,
			INFO,
			WARN,
			ERROR
		};

		explicit Logger(std::string_view name, bool addStdoutSink = true);

		void Log(Level level, std::string_view message, std::optional<std::source_location> location = std::nullopt);

		template <typename... Args>
		void Info(std::format_string<Args...> format, Args&&... args) {
			Log(Level::INFO, std::format(format, std::forward<Args>(args)...));
		}
		
		template <typename... Args>
		void Warn(std::format_string<Args...> format, Args&&... args) {
			Log(Level::WARN, std::format(format, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void Error(std::format_string<Args...> format, Args&&... args) {
			Log(Level::ERROR, std::format(format, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void Debug(std::format_string<Args...> format, Args&&... args) {
			Log(Level::DEBUG, std::format(format, std::forward<Args>(args)...));
		}
		
		template <typename... Args>
		void InfoTrace(const std::source_location& location, std::format_string<Args...> format, Args&&... args) {
			Log(Level::INFO, std::format(format, std::forward<Args>(args)...), location);
		}
		
		template <typename... Args>
		void WarnTrace(const std::source_location& location, std::format_string<Args...> format, Args&&... args) {
			Log(Level::WARN, std::format(format, std::forward<Args>(args)...), location);
		}

		template <typename... Args>
		void ErrorTrace(const std::source_location& location, std::format_string<Args...> format, Args&&... args) {
			Log(Level::ERROR, std::format(format, std::forward<Args>(args)...), location);
		}

		template <typename... Args>
		void DebugTrace(const std::source_location& location, std::format_string<Args...> format, Args&&... args) {
			Log(Level::DEBUG, std::format(format, std::forward<Args>(args)...), location);
		}

		static Logger& GetGlobalLogger(std::string_view name = "Conveyor");
	private:
		static std::string GetFormattedTime();
		static std::string GetFormattedSource(const std::source_location& location);

		std::string m_Name;
		std::vector<std::shared_ptr<LoggerSink>> m_Sinks;
	};
}