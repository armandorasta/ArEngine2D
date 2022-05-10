#pragma once

#include "Vec2.h"
#include "Testing/ArTest20.h"

#include <vector>

namespace ArFac {
	using namespace ArEngine2D;

	namespace DataGridDetails {

		template <class T>
		concept ValidDataGridSizeType = true;
	}
	
	struct DataGridCoord
	{
		std::size_t x;
		std::size_t y;
	};

	/**
	 * @brief basically a two dimensional array.
	 * @tparam TElement => the type of the stored data.
	*/
	template <class TElement>
	class DataGrid
	{
	private:
		using self = DataGrid;

	public:

		using Coord = DataGridCoord;

	public:

		DataGrid(self const&)		 = default;
		DataGrid(self&&)			 = default;
		self& operator=(self const&) = default;
		self& operator=(self&&)		 = default;

		/**
		 * @brief this will initialize primitive types.
		 * @param width => the number of columns.
		 * @param height => the number of rows.
		*/
		constexpr DataGrid(std::size_t width, std::size_t height) noexcept
			: width_{width}, height_{height} { data_.resize(Size()); }

		/**
		 * @brief this will initialize primitive types.
		 * @param width => the number of columns.
		 * @param height => the number of rows.
		 * @param val => initial value for every element in the grid.
		*/
		constexpr DataGrid(std::size_t width, std::size_t height, TElement const& val) noexcept
			: DataGrid{width, height} 
		{ 
			Fill(val);
		}

	public:

		/**
		 * @return the 2d coordinate equivalent of the index passed.
		*/
		constexpr Coord IndexToCoord(std::size_t index) const noexcept
		{ return {index % width_, index / width_}; }

		/**
		 * @return the index equivalent of the 2d coordinate passed.
		*/
		constexpr std::size_t CoordToIndex(std::size_t col, std::size_t row) const noexcept
		{ return col + row * width_; }
		
		/**
		 * @return the index equivalent of the 2d coordinate passed.
		*/
		constexpr std::size_t CoordToIndex(Coord coord) const noexcept
		{ return CoordToIndex(coord.x, coord.y); }
		
		/**
		 * @brief calling this with an invalid index is undefined.
		 * @return a reference to the element at that index.
		*/
		constexpr TElement& AtIndex(std::size_t index) noexcept
		{ 
			assert(index < Size());
			return data_[index]; 
		}

		/**
		 * @brief calling this with an invalid index is undefined.
		 * @return a reference to the element at that index.
		*/
		constexpr TElement const& AtIndex(std::size_t index) const noexcept
		{ return const_cast<self&>(*this).AtIndex(index); }
		
		/**
		 * @brief calling this with an invalid col or row is undefined.
		 * @return a reference to the element at that column and row.
		*/
		constexpr TElement& At(std::size_t col, std::size_t row) noexcept
		{ return AtIndex(CoordToIndex(col, row)); }

		/**
		 * @brief calling this with an invalid col or row is undefined.
		 * @return a reference to the element at that column and row.
		*/
		constexpr TElement const& At(std::size_t col, std::size_t row) const noexcept
		{ return const_cast<self&>(*this).AT(col, row); }

		/**
		 * @brief calling this with an invalid Coord is undefined.
		 * @return a reference to the element at that coordinate.
		*/
		constexpr TElement& At(Coord const& where) noexcept
		{ return At(where.x, where.y); }
		
		/**
		 * @brief calling this with an invalid Coord is undefined.
		 * @return a reference to the element at that coordinate.
		*/
		constexpr TElement const& At(Coord const& where) const noexcept
		{ return const_cast<self&>(*this).At(where); }

		/**
		 * @brief calling this with an invalid index is undefined.
		 * @return a reference to the element at that index.
		*/
		constexpr TElement& operator[](std::size_t index) noexcept
		{ return AtIndex(index); }

		/**
		 * @brief calling this with an invalid index is undefined.
		 * @return a reference to the element at that index.
		*/
		constexpr TElement const& operator[](std::size_t index) const noexcept
		{ return AtIndex(index); }

		/**
		 * @brief calling this with an invalid col or row is undefined.
		 * @return a reference to the element at that column and row.
		*/
		constexpr TElement& operator()(std::size_t col, std::size_t row) noexcept
		{ return At(col, row); }

		/**
		 * @brief calling this with an invalid Coord is undefined.
		 * @return a reference to the element at that coordinate.
		*/
		constexpr TElement const& operator()(std::size_t col, std::size_t row) const noexcept
		{ return At(col, row); }

		/**
		 * @param col => column (x).
		 * @param row => row (y).
		 * @return true if the passed column and row are within the grid, false otherwise.
		*/
		constexpr bool IsInside(std::size_t col, std::size_t row) const noexcept
		{ return ((col < width_) && (row < height_)); }
		
		/**
		 * @param what => 2d coordinate.
		 * @return true if the passed column and row are within the grid, false otherwise.
		*/
		constexpr bool IsInside(Coord const& what) const noexcept
		{ return IsInside(what.x, what.y); }

		/**
		 * @brief using a non-exitstant row is undefined.
		*/
		constexpr void FillRow(std::size_t rowIndex, TElement const& withWhat) noexcept
		{  
			assert(rowIndex < Height());
			std::fill_n(data_.begin() + CoordToIndex(0, rowIndex), width_, withWhat);
		}

		/**
		 * @brief using a non-exitstant column is undefined.
		*/
		constexpr void FillCol(std::size_t colIndex, TElement const& withWhat) noexcept
		{  
			assert(colIndex < Width());
			for (auto i{CoordToIndex(colIndex, 0)}, lim{CoordToIndex(colIndex, height_-1)};
				i < lim; i += width_)
			{
				AtIndex(i) = withWhat;
			}
		}

		/**
		 * @brief fills the grid with the value passed.
		*/
		constexpr void Fill(TElement const& withWhat) noexcept
		{ std::ranges::fill(data_, withWhat); }

		/**
		 * @brief using a non-exitstant row is undefined.
		*/
		constexpr bool IsRowFilledWith(std::size_t rowIndex, TElement const& what) const noexcept
		{ 
			assert(rowIndex < Height());
			auto const it{data_.begin() + CoordToIndex(0, rowIndex)};
			return std::all_of(it, it + width_, [&what](auto&& el) { return el == what; });
		}

		/**
		 * @brief using a non-exitstant column is undefined.
		*/
		constexpr bool IsColFilledWith(std::size_t colIndex, TElement const& what) const noexcept
		{
			for (auto i{CoordToIndex(colIndex, 0)}, lim{CoordToIndex(colIndex, height_-1)};
				i < lim; i += width_)
			{
				if (AtIndex(i) != what)
				{
					return false;
				}
			}
			return true;
		}

		/**
		 * @return true if every element in the grid compares equal to the one passed in.
		*/
		constexpr bool IsFilledWith(TElement const& what) const noexcept
		{
			return std::ranges::all_of(data_, [&what](auto&& el) { return el == what; });
		}

		/**
		 * @return true if the width and height of the grid are the same.
		*/
		constexpr bool IsSquare() const noexcept
		{ return width_ == height_; }

	public:

		// no checks
		constexpr void SetWidth(std::size_t newWidth) noexcept
		{ width_ = newWidth; }
		// no checks
		constexpr void SetHeight(std::size_t newHeight) noexcept
		{ height_ = newHeight; }
		
		constexpr std::size_t Width() const noexcept
		{ return width_; }

		constexpr std::size_t Height() const noexcept
		{ return height_; }
		
		constexpr std::size_t Size() const noexcept
		{ return width_ * height_; }
		
		constexpr TElement* Data() noexcept
		{ return data_.data(); }

		constexpr TElement const* Data() const noexcept
		{ return data_.data(); }

	private:
		std::size_t width_;
		std::size_t height_;
		std::vector<TElement> data_;
	};

	inline void TestDataGrid() 
	{
		using namespace ArTest;
		std::ofstream file{"DataGridTestResults.txt"};
		Tester tester{file};

		tester.NewTest("Constructors") = [&] {
			DataGrid<float> grido{3, 4};
			tester.PassIfEqual(grido.Width(), 3);
			tester.PassIfEqual(grido.Height(), 4);

			DataGrid<float> gridina{2, 3, 5.f};
			tester.PassIf(gridina.IsFilledWith(5.f));
		};

		tester.Namespace("Filling") = [&] {
			DataGrid<int> grido{2, 3};

			tester.NewTest("Fill IsFilledWith") = [&] {
				grido.Fill(5);
				tester.PassIf(grido.IsFilledWith(5));
			};
			
			tester.NewTest("IsRowFilledWith and IsColFilledWith") = [&] {
				tester.PassIf(grido.IsRowFilledWith(0, 5));
				tester.PassIf(grido.IsRowFilledWith(1, 5));
				tester.PassIf(grido.IsRowFilledWith(2, 5));
				tester.PassIf(grido.IsColFilledWith(0, 5));
				tester.PassIf(grido.IsColFilledWith(1, 5));
				tester.PassIf(grido.IsColFilledWith(2, 5));
			};

			tester.NewTest("FillRow") = [&] {
				grido.FillRow(2, 10);
				tester.PassIf(grido.IsRowFilledWith(2, 10));
			};

			tester.NewTest("FillCol") = [&] {
				grido.FillCol(2, 10);
				tester.PassIf(grido.IsColFilledWith(2, 10));
			};
		};

		tester.OutputResults();
	}
}