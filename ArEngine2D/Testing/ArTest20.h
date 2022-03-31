#pragma once
#ifndef AR_TEST_LIB_H_
#define AR_TEST_LIB_H_

#include <source_location>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <format>
#include <unordered_set>
#include <optional>

namespace ArTest {
	class Test;

	namespace Anthony {
		template <class TOperand, class TOperator>
		concept ImplementsOperator = requires(TOperand const& t)
		{
			{ TOperator{}(t, t) } -> std::convertible_to<bool>;
		};

		/**
		 * @brief not copyable nor movable.
		*/
		class ISingle
		{
		public:
			ISingle() = default;
			ISingle(ISingle const&) = delete;
			ISingle(ISingle&&) = delete;
			ISingle& operator=(ISingle const&) = delete;
			ISingle& operator=(ISingle&&) = delete;
		};

		/**
		 * @brief used for all exceptions thrown by the library.
		*/
		class TestError final : public std::exception
		{
		public:

			TestError(std::string_view message) noexcept
				: exception{message.data()}
			{ }
		};

		/**
		 * @brief makes the namespace syntax nice :)
		*/
		class NamespaceProxy final : ISingle
		{
		private:

			using Callable = std::function<void()>;

		public:

			NamespaceProxy(Callable&& pushCall, Callable&& popCall) :
				push_{std::move(pushCall)}, pop_{std::move(popCall)}
			{ }

			void operator=(std::function<void()>&& body)&&
			{
				push_();
				body();
				pop_();
			}

		private:
			Callable push_;
			Callable pop_;
		};

		class TestProxy final : ISingle
		{
		private:

			using Callable = std::function<void()>;

		public:

			TestProxy() = default;
			~TestProxy() = default;

			void operator=(Callable&& body)&&
			{ body(); }
		};

	} // namespace Details

	class Test final
	{
	public:

		constexpr Test() : bState_{true} { }
		constexpr Test(bool initialState) : bState_{initialState} { }

	public:

		constexpr void Pass()
		{ bState_ = true; }

		constexpr void Fail()
		{ bState_ = false; }

	public:

		constexpr Test& operator<<(bool rhs)
		{
			bState_ &= rhs;
			return *this;
		}

	public:

		constexpr bool IsPassed() const
		{ return bState_; }
		constexpr bool IsFailed() const
		{ return not IsPassed(); }

	private:
		bool bState_;
	};

	struct TestInfo final : Anthony::ISingle
	{
		constexpr TestInfo(std::string_view name, std::vector<std::string> const& namespaces)
			: Name{name}, Namespaces{namespaces}
		{ }

		std::string_view Name;
		std::vector<std::string> const& Namespaces;
	};

	enum class BinaryOperatorType
	{
		INVALID,
		EQUAL,
		NOT_EQUAL,
		GREATER,
		GREATER_EQUAL,
		LESS,
		LESS_EQUAL,
	};

	class IFormatter
	{
	public:

		enum class FunctionFailedType
		{
			FAIL_UNCONDITIONALLY,
			PASS_IF_CONDITION,
			PASS_IF_NOT,
			PASS_IF_BINARY_OPERATOR,
		};


		struct FailureInfo
		{
			constexpr FailureInfo(FunctionFailedType funcType, std::optional<std::string_view> lhs, std::optional<std::string_view> rhs,
				BinaryOperatorType opType, std::source_location debugInfo) :
				FailedFunc{funcType}, Lhs{lhs}, Rhs{rhs}, OperatorType{opType}, DebugInfo{debugInfo}
			{ }

			// PassIf, Fail, PassIfGreaterEq...
			FunctionFailedType FailedFunc;

			// have values only with binary operators.
			std::optional<std::string> Lhs;
			std::optional<std::string> Rhs;
			std::optional<BinaryOperatorType> OperatorType;

			std::source_location DebugInfo;
		};

		struct TestFormattingInfo
		{
			constexpr TestFormattingInfo(std::string_view testName, std::vector<std::string> const& namespaces,
				std::optional<FailureInfo> failureInfo) :
				TestName{testName}, Namespaces{namespaces}, FailureInfo{failureInfo}
			{}

			std::string TestName;
			std::vector<std::string> Namespaces;

			// has value if the test failed.
			std::optional<FailureInfo> FailureInfo;
		};

		using FormatArgs = TestInfo const&;
		using FormatOnceArgs = TestFormattingInfo;

	public:

		constexpr IFormatter() = default;
		virtual ~IFormatter() = default;

	public:

		virtual void Initialize(std::vector<FormatOnceArgs> const&) { }
		virtual std::string FormatTestName(std::vector<std::string> const& namespaces, std::string_view testName) = 0;
		virtual std::string FormatPassed() = 0;
		virtual std::string FormatFailedUnconditionally(std::source_location debugInfo) = 0;
		virtual std::string FormatPassIfCondition(std::source_location debugInfo) = 0;
		virtual std::string FormatPassIfNot(std::source_location debugInfo) = 0;
		virtual std::string FormatPassIfBinaryOperator(std::string_view lhs, std::string_view rhs, BinaryOperatorType opType,
			std::source_location debugInfo) = 0;
		virtual void Finalize() noexcept { }

		void Format(std::vector<FormatOnceArgs> args, std::ostream& os)
		{
			Initialize(args);

			for (size_t i{}; i < args.size(); ++i)
			{
				os << FormatTestName(args[i].Namespaces, args[i].TestName);
				auto const test{args[i]};
				if (auto const& info{test.FailureInfo}; not info)
				{
					os << std::format(" {}\n", FormatPassed());
				}
				else switch (info->FailedFunc)
				{
					using enum FunctionFailedType;
				case FAIL_UNCONDITIONALLY:
				{
					os << std::format(" {}\n", FormatFailedUnconditionally(test.FailureInfo->DebugInfo));
					break;
				}

				case PASS_IF_CONDITION:
				{
					os << std::format(" {}\n", FormatPassIfCondition(test.FailureInfo->DebugInfo));
					break;
				}

				case PASS_IF_NOT:
				{
					os << std::format(" {}\n", FormatPassIfNot(test.FailureInfo->DebugInfo));
					break;
				}

				case PASS_IF_BINARY_OPERATOR:
				{
					os << std::format(" {}\n", FormatPassIfBinaryOperator(*info->Lhs, *info->Rhs, *info->OperatorType, info->DebugInfo));
					break;
				}

				default: /*can't happen*/ throw Anthony::TestError{"invalid FailedFunctionType"};
				}
			}
		}
	};

	namespace Anthony {
		class DefaultFormatter final : public IFormatter
		{
		public:

			using IFormatter::IFormatter;

		public:

			void Initialize(std::vector<FormatOnceArgs> const& args) override
			{
				// well contain the test names mashed together with their namespaces in one string each.
				std::vector<std::string> readyMeals{};
				readyMeals.reserve(std::size(args));

				for (auto const& arg : args)
				{
					auto& str{readyMeals.emplace_back()};
					for (auto const& ns : arg.Namespaces)
					{
						str += ns + ": ";
					}
					str += std::format("'{}'", arg.TestName);

					// longest name yet?
					maxWidth_ = std::max(maxWidth_, std::size(str));
				}
			}

			std::string FormatTestName(std::vector<std::string> const& namespaces, std::string_view name) override
			{
				std::string res{};
				for (auto const& ns : namespaces)
				{
					res += ns + ": ";
				}
				res += std::string{name} + " ";
				for (size_t j{}, currWidth{maxWidth_ - res.size()}; j <= currWidth; ++j)
				{
					res += '>';
				}
				return res;
			}

			std::string FormatPassed() override
			{ return "[Passed]"; }

			std::string FormatFailedUnconditionally(std::source_location debugInfo) override {
				return std::format(" [Failed (line: {})] {{\n\tfailed unconditionally.\n}}", debugInfo.line());
			}

			std::string FormatPassIfCondition(std::source_location debugInfo) override {
				return std::format(" [Failed (line: {})] {{\n\texpected true, but got false.\n}}", debugInfo.line());
			}
			
			std::string FormatPassIfNot(std::source_location debugInfo) override {
				return std::format(" [Failed (line: {})] {{\n\texpected false, but got true.\n}}", debugInfo.line());
			}

			std::string FormatPassIfBinaryOperator(std::string_view lhs, std::string_view rhs, BinaryOperatorType opType,
				std::source_location debugInfo) override
			{
				auto const reason{OperatorTypeToMessage(opType)};
				return std::format(
					" [Failed (line: {})] {{\n"
					"\tlhs: {}\n"
					"\trhs: {}\n"
					"\tReason: {}\n"
					"}}",
					debugInfo.line(), lhs, rhs, reason);
			}

		private:

			static std::string OperatorTypeToMessage(BinaryOperatorType opType)
			{
				using enum BinaryOperatorType;
				static std::unordered_map<BinaryOperatorType, std::string> strMap{
					{EQUAL,			"expected both sides to compare equal."},
					{NOT_EQUAL,		"expected both sides to not compare equal."},
					{GREATER,		"expected lhs to have a value compares greater than rhs."},
					{GREATER_EQUAL,	"expected lhs to have a value compares greater than or equal to rhs."},
					{LESS,			"expected lhs to have a value compares less than rhs."},
					{LESS_EQUAL,	"expected lhs to have a value compares less than or equal to rhs."},
				};

				return strMap.at(opType);
			}

		private:
			std::size_t maxWidth_{};
		};
	}

	class Tester final : Anthony::ISingle
	{
	private:

		struct FormatterDeleter
		{
			constexpr FormatterDeleter(bool bOnTheHeap) : bDelete{bOnTheHeap} { }
			constexpr void operator()(IFormatter* ptr)
			{
				if (bDelete)
				{
					delete ptr;
				}
			}
			bool bDelete;
		};

	public:

		Tester(std::ostream& outStream) :
			outStream_{outStream}, pFmt_{new Anthony::DefaultFormatter{}, true} // call delete
		{ }

		Tester(std::ostream& outStream, IFormatter* formatter) :
			outStream_{outStream}, pFmt_{formatter, false} // don't call delete
		{ }

		~Tester()
		{
			pFmt_->Finalize();
		}

	public:

		[[nodiscard]]
		Anthony::NamespaceProxy Namespace(std::string_view name)
		{
			// proxy will use this to before calling the namespace "body"
			auto const pushNewNamespace = [&]() constexpr {
				namespaces_.push_back(std::string{name});
			};

			// proxy will use this to after calling the namespace "body"
			auto const popLastNamespace = [&]() constexpr {
				namespaces_.pop_back();
			};

			return Anthony::NamespaceProxy{pushNewNamespace, popLastNamespace};
		}

		[[nodiscard]]
		Anthony::TestProxy NewTest(std::string_view name)
		{
			// it seems contridictory, but it's the right behaviour.
			currTest_.Pass();
			fmtOnceInfo_.push_back({name, namespaces_, {}});

			/*
				TODO: change this way of finding a doublicate test because,
					  the formatter does not have to format the same test
					  in the exact same way every time.

					  for now formatters are required to do that, but later
					  they won't be.
			*/

			auto const hashedName{HashTestName(name)};
			if (allTests_.contains(hashedName))
			{
				throw Anthony::TestError{std::format("test: {} was defined more than once", name)};
			}

			// test must be valid by this point
			allTests_.emplace(hashedName);

			return {};
		}

		void OutputResults()
		{
			pFmt_->Format(fmtOnceInfo_, outStream_);
		}

	public:

		constexpr Tester& Fail(std::source_location debugInfo = std::source_location::current())
		{
			if (currTest_.IsFailed()) // test failed already? no need to do any more work.
			{
				return *this;
			}

			currTest_.Fail();
			fmtOnceInfo_.back().FailureInfo = {
				IFormatter::FunctionFailedType::FAIL_UNCONDITIONALLY,
				{/*no lhs*/},
				{/*no rhs*/},
				{/*no reason*/},
				debugInfo
			};

			return *this;
		}

		constexpr Tester& PassIf(bool condition, std::source_location debugInfo = std::source_location::current())
		{
			if (currTest_.IsFailed()) // test failed already? no need to do any more work.
			{
				return *this;
			}

			if (not condition) // test failed
			{
				fmtOnceInfo_.back().FailureInfo = {
					IFormatter::FunctionFailedType::PASS_IF_CONDITION,
					{/*no lhs*/},
					{/*no rhs*/},
					{/*no reason*/},
					debugInfo
				};
				currTest_.Fail();
			}

			return *this;
		}

		constexpr Tester& PassIfNot(bool condition, std::source_location debugInfo = std::source_location::current())
		{ // can't just call PassIf(not condition) because, the message is different.
			if (currTest_.IsFailed()) // test failed already? no need to do any more work.
			{
				return *this;
			}

			if (condition) // test failed
			{
				fmtOnceInfo_.back().FailureInfo = {
					IFormatter::FunctionFailedType::PASS_IF_NOT,
					{/*no lhs*/},
					{/*no rhs*/},
					{/*no reason*/},
					debugInfo
				};
			
				currTest_.Fail();
			}

			return *this;
		}

#define AR_TEST_LIB_PASSIF_HELPER_MAC(_opName, _funcName, _enumType) \
		template <Anthony::ImplementsOperator<std::_opName<>> TLhs, std::convertible_to<TLhs> TRhs> \
		constexpr decltype(auto) _funcName(TLhs&& lhs, TRhs&& rhs, \
			std::source_location debugInfo = std::source_location::current()) \
		{ \
			return PassIfHelper<std::_opName<>, TLhs, BinaryOperatorType::_enumType>( \
				std::_opName<>{}, std::forward<TLhs>(lhs), std::forward<TLhs>(rhs), debugInfo \
			); \
		}

		AR_TEST_LIB_PASSIF_HELPER_MAC(equal_to, PassIfEqual, EQUAL)
		AR_TEST_LIB_PASSIF_HELPER_MAC(not_equal_to, PassIfNotEqual, NOT_EQUAL)
		AR_TEST_LIB_PASSIF_HELPER_MAC(greater, PassIfGreater, GREATER)
		AR_TEST_LIB_PASSIF_HELPER_MAC(greater_equal, PassIfGreaterEq, GREATER_EQUAL)
		AR_TEST_LIB_PASSIF_HELPER_MAC(less, PassIfLess, LESS)
		AR_TEST_LIB_PASSIF_HELPER_MAC(less_equal, PassIfLessEq, LESS_EQUAL)

#undef AR_TEST_LIB_PASSIF_HELPER_MAC

	private:

		template <class TOperator, class TOperand, BinaryOperatorType OperatorType>
		Tester& PassIfHelper(TOperator&& op, TOperand&& lhs, TOperand&& rhs, std::source_location const& debugInfo)
		{
			if (currTest_.IsFailed()) // test failed already? no need to do any more work.
			{
				return *this;
			}

			// do the test
			currTest_ << op(lhs, rhs);
			if (currTest_.IsFailed()) // test JUST failed?
			{
				auto const lString{std::format("{}", lhs)};
				auto const rString{std::format("{}", rhs)};
				
				fmtOnceInfo_.back().FailureInfo = {
					IFormatter::FunctionFailedType::PASS_IF_BINARY_OPERATOR,
					{std::format("{}", lhs)},
					{std::format("{}", rhs)},
					OperatorType,
					debugInfo,
				};
			}

			return *this;
		}

	private:

		std::string HashTestName(std::string_view testName)
		{
			std::string res{};
			for (auto const& ns : namespaces_)
			{
				res += ns + "-";
			}
			return res + std::string{testName};
		}

	private:
		std::ostream& outStream_;
		Test currTest_{};
		std::unique_ptr<IFormatter, FormatterDeleter> pFmt_;

		std::vector<std::string> namespaces_;
		std::unordered_set<std::string> allTests_;

		std::vector<IFormatter::TestFormattingInfo> fmtOnceInfo_;
	};

	inline namespace GlobalImpl {
		namespace InnerAnthony {
			inline Tester* g_pCurrentTester{};

			inline void CheckGlobalRef()
			{
				if (not g_pCurrentTester)
				{
					throw ::ArTest::Anthony::TestError{"tried to use a global function before calling SetGlobalTesterRefTo"};
				}
			}
		}

		inline void SetGlobalTesterRefTo(Tester* pTester)
		{
			InnerAnthony::g_pCurrentTester = pTester;
		}

		inline void Fail(std::source_location debugInfo = std::source_location::current())
		{
			InnerAnthony::CheckGlobalRef();
			InnerAnthony::g_pCurrentTester->Fail(debugInfo);
		}

		inline void PassIf(bool condition, std::source_location debugInfo = std::source_location::current())
		{
			InnerAnthony::CheckGlobalRef();
			InnerAnthony::g_pCurrentTester->PassIf(condition, debugInfo);
		}

#define AR_TEST_LIB_GLOBAL_IMPL_HELPER_MAC(_funcName) \
		template <class TLhs, class TRhs> \
		constexpr inline void _funcName(TLhs const& lhs, TRhs const& rhs, \
			std::source_location debugInfo = std::source_location::current()) \
		{ \
			InnerAnthony::CheckGlobalRef();	\
			InnerAnthony::g_pCurrentTester->_funcName(lhs, static_cast<TLhs>(rhs), debugInfo); \
		}

		AR_TEST_LIB_GLOBAL_IMPL_HELPER_MAC(PassIfEqual)
			AR_TEST_LIB_GLOBAL_IMPL_HELPER_MAC(PassIfNotEqual)
			AR_TEST_LIB_GLOBAL_IMPL_HELPER_MAC(PassIfGreater)
			AR_TEST_LIB_GLOBAL_IMPL_HELPER_MAC(PassIfGreaterEq)
			AR_TEST_LIB_GLOBAL_IMPL_HELPER_MAC(PassIfLess)
			AR_TEST_LIB_GLOBAL_IMPL_HELPER_MAC(PassIfLessEq)

#undef AR_TEST_LIB_GLOBAL_IMPL_HELPER_MAC

			inline decltype(auto) Namespace(std::string_view name)
		{
			InnerAnthony::CheckGlobalRef();
			return InnerAnthony::g_pCurrentTester->Namespace(name);
		}

		inline decltype(auto) NewTest(std::string_view name)
		{
			InnerAnthony::CheckGlobalRef();
			return InnerAnthony::g_pCurrentTester->NewTest(name);
		}
	}
} // ArTest

#endif // AR_TESTING_LIB_OLD_H_

