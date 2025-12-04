#pragma once

#ifdef TDHELPER_EXPORTS
#define TDHELPER_API __declspec(dllexport)
#else
#define TDHELPER_API __declspec(dllimport)
#endif

#include <random>
#include <type_traits>

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };
constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

constexpr int err_pointer{ 1111 };
constexpr int err_index{ 1112 };
constexpr int err_data{ 1113 };
constexpr int err_wrong_copy{ 1114 };

constexpr unsigned char no_flag{ 0 };
constexpr unsigned char left_flag{ 0b00000001 };
constexpr unsigned char right_flag{ 0b00000010 };
constexpr unsigned char up_flag{ 0b00000100 };
constexpr unsigned char down_flag{ 0b00001000 };

constexpr unsigned char up_left_flag{ 0b00000101 };
constexpr unsigned char up_right_flag{ 0b00000110 };
constexpr unsigned char down_left_flag{ 0b00001001 };
constexpr unsigned char down_right_flag{ 0b00001010 };

enum class dirs { up = 0, right = 1, left = 2, down = 3, stop = 4, up_left = 5, up_right = 6, down_left = 7, down_right = 8 };
enum class states { idle = 0, attack = 1, heal = 2 };

enum class orcs { warrior1 = 0, warrior2 = 1, crusher = 2, healer = 3, flyer = 4, champion = 5 };
enum class buildings {
	arhcer = 0, small_cannon = 1, mid_cannon = 2, big_cannon = 3, small_mage = 4,
	mid_mage = 5, big_mage = 6, castle = 7, wall = 8
};
enum class assets { rock = 0, small_tree = 1, mid_tree = 2, big_tree = 3 };
enum class shots { arrow = 0, cannonball = 1, spell = 2 };

struct TDHELPER_API FPOINT
{
	float x{ 0 };
	float y{ 0 };
};

struct TDHELPER_API FRECT
{
	float left{ 0 };
	float right{ 0 };
	float up{ 0 };
	float down{ 0 };
};

namespace dll
{
	class TDHELPER_API EXCEPTION
	{
	private:
		const wchar_t* pointer_error{ L"Error in base pointer !" };
		const wchar_t* index_error{ L"Error in index passed !" };
		const wchar_t* data_error{ L"Invalid data read !" };
		const wchar_t* copy_error{ L"Error in copy parameter!" };
		const wchar_t* unknown_error{ L"Unknown error !" };

		int error_occurred = -1;

	public:
		EXCEPTION(int what_happend)
		{
			error_occurred = what_happend;
		}

		const wchar_t* GetMsg()
		{
			switch (error_occurred)
			{
			case err_pointer:
				return pointer_error;

			case err_index:
				return index_error;

			case err_data:
				return data_error;

			case err_wrong_copy:
				return copy_error;
			}
		
			return unknown_error;
		}
	};
	
	template<typename T>concept primes = (std::is_same<T, int>::value || std::is_same<T, float>::value
		|| std::is_same<T, double>::value || std::is_same<T, char>::value);

	template<typename T> class BAG
	{
	private:
		T* m_ptr{ nullptr };
		size_t max_size{ 0 };
		size_t next_pos{ 0 };
		bool is_valid{ false };

	public:

		BAG() :m_ptr{ reinterpret_cast<T*>(calloc(1,sizeof(T))) } 
		{
			max_size = 1;
		};
		BAG(size_t lenght) :m_ptr{ reinterpret_cast<T*>(calloc(lenght,sizeof(T))) }
		{
			max_size = lenght;
		}

		BAG(BAG& other)
		{
			if (m_ptr != other->m_ptr)
			{
				if (!other->m_ptr || other->empty())throw EXCEPTION(err_wrong_copy);

				free(m_ptr);

				m_ptr = nullptr;

				max_size = other->max_size;
				next_pos = other->next_pos;

				m_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
				if (!m_ptr)throw EXCEPTION(err_pointer);

				for (size_t i = 0; i < next_pos; ++i)m_ptr[i] = other[i];
			}
		}
		BAG(BAG&& other)
		{
			if (m_ptr != other->m_ptr)
			{
				if (!other->m_ptr || other->empty())throw EXCEPTION(err_wrong_copy);

				free(m_ptr);

				m_ptr = other->m_ptr;

				max_size = other->max_size;
				next_pos = other->next_pos;

				other->m_ptr = nullptr;
			}
		}

		~BAG()
		{
			free(m_ptr);
		}

		bool empty()const
		{
			if (is_valid)return false;

			return true;
		}
		size_t size()const
		{
			return next_pos;
		}
		size_t capacity()const
		{
			return max_size;
		}

		void push_back(T what)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = what;
					++next_pos;
					is_valid = true;
					return;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * max_size));
					
					if (!m_ptr)throw EXCEPTION(err_pointer);
					else
					{
						m_ptr[next_pos] = what;
						++next_pos;
						is_valid = true;
						return;
					}
				}
			}
		}
		void push_back(T* what)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = (*what);
					++next_pos;
					is_valid = true;
					return;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * max_size));

					if (!m_ptr)throw EXCEPTION(err_pointer);
					else
					{
						m_ptr[next_pos] = (*what);
						++next_pos;
						is_valid = true;
						return;
					}
				}
			}
		}

		void push_front(T what)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			else
			{
				(*m_ptr) = what;
				is_valid = true;
			}
		}
		void push_front(T* what)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			else
			{
				(*m_ptr) = (*what);
				is_valid = true;
			}
		}

		void erase(size_t index)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			if (index < 0 || index >= next_pos)throw EXCEPTION(err_index);
			if (empty())throw EXCEPTION(err_data);

			T* temp_ptr{ reinterpret_cast<T*>(calloc(max_size - 1,sizeof(T))) };
			for (int i = 0; i < next_pos; ++i)
			{
				if (i < index)temp_ptr[i] = m_ptr[i];
				else temp_ptr[i] = m_ptr[i + 1];
			}

			free(m_ptr);
			m_ptr = temp_ptr;
			--max_size;
			--next_pos;
		}

		BAG& operator=(BAG& other)
		{
			if (!m_ptr || !other->m_ptr)throw EXCEPTION(err_pointer);
			else
			{
				if (other->empty())throw EXCEPTION(err_data);

				if (m_ptr != other->m_ptr)
				{
					free(m_ptr);

					max_size = other->max_size;
					next_pos = other->next_pos;

					m_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

					if (!m_ptr)throw EXCEPTION(err_pointer);
					else
						for (size_t i = 0; i < next_pos; ++i)m_ptr[i] = other->m_ptr[i];
				}

			}

			return *this;
		}
		BAG& operator=(BAG&& other)
		{
			if (!other->m_ptr)throw EXCEPTION(err_pointer);

			if (m_ptr != other->m_ptr)
			{
				free(m_ptr);

				max_size = other->max_size;
				next_pos = other->next_pos;

				m_ptr = other->m_ptr;

				other->m_ptr = nullptr;
			}

			return *this;
		}


		T& operator[](size_t index)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			if (index < 0 || index >= next_pos)throw EXCEPTION(err_index);
			if (empty())throw EXCEPTION(err_data);

			return m_ptr[index];
		}
	};

	//////////////////////////////////////////////////////////////////

	class TDHELPER_API RANDIT
	{
	private:
		std::mt19937* twister{ nullptr };

	public:
		RANDIT();
		~RANDIT();

		int operator()(int min, int max);
	};

	class TDHELPER_API PROTON
	{
	protected:
		float _width{ 0 };
		float _height{ 0 };

	public:
		FPOINT start{};
		FPOINT end{};
		FPOINT center{};

		float x_radius{ 0 };
		float y_radius{ 0 };

		PROTON();
		PROTON(float sx, float sy);
		PROTON(float sx, float sy, float swidth, float sheight);

		virtual ~PROTON() {};

		void new_dims(float new_width, float new_height);
		void set_edges();

		float get_width()const;
		float get_height()const;

		void set_width(float new_width);
		void set_height(float new_height);
	};

	class TDHELPER_API ASSETS :public PROTON
	{
	private:
		assets _type{ assets::rock };

		ASSETS(assets _what_type, float _start_x, float _start_y);

	public:
		
		assets get_type()const;

		void Release();

		friend TDHELPER_API ASSETS* AssetFactory(assets what_type, float start_x, float start_y);
	};

	class TDHELPER_API SHOTS :public PROTON
	{
	private:
		float move_sx{ 0 };
		float move_sy{ 0 };
		float move_ex{ 0 };
		float move_ey{ 0 };

		float slope{ 0 };
		float intercept{ 0 };

		bool ver_dir{ false };
		bool hor_dir{ false };

		shots _type{ shots::arrow };

		int current_frame{ 0 };
		int max_frames{ 63 };

		int strenght = 1;
		
		void SetPathInfo(float _ex, float _ey);

		SHOTS(shots _what, float _where_x, float _where_y, 
			float _to_where_x, float _to_where_y, int _shot_modifier);

	public:
		dirs dir{ dirs::stop };
		float speed = 1.0f;

		bool move();

		int get_frame();
		int get_strenght()const;

		shots get_type()const;

		void Release();

		friend TDHELPER_API SHOTS* ShotFactory(shots what, float where_x, float where_y, 
			float to_where_x, float to_where_y, int shot_modifier);
	};

	class TDHELPER_API BUILDINGS :public PROTON
	{
	private:
		buildings _type{ buildings::arhcer };
		int strenght{ 0 };
		float range{ 0 };

		int frame{ 0 };
		int max_frames{ 0 };
		int frame_delay{ 0 };

		int fire_rate{ 0 };
		int fire_status{ 0 };

		BUILDINGS(buildings _what, float _sx, float _sy);

	public:
		int lifes{ 0 };

		buildings get_type()const;
		void set_type(buildings what);

		int attack();
		int get_frame();
		float get_range() const;

		void Release();

		friend TDHELPER_API BUILDINGS* BuildingFactory(buildings what, float sx, float sy);
	};

	class TDHELPER_API ORCS :public PROTON
	{
	private:
		orcs _type{ orcs::warrior1 };

		static unsigned int _counter;
		int _my_number{ 0 };

		int frame{ 0 };
		int max_frames{ 0 };
		int frame_delay{ 0 };

		float move_sx{ 0 };
		float move_sy{ 0 };
		float move_ex{ 0 };
		float move_ey{ 0 };
		float slope{ 0 };
		float intercept{ 0 };

		bool hor_dir = false;
		bool ver_dir = false;

		float speed{ 0 };

		int attack_delay{ 0 };
		int strenght{ 0 };

		int max_lifes{ 0 };

		ORCS(orcs _what, float _sx, float _sy);

	public:
		dirs dir{ dirs::stop };
		states state = states::idle;
		
		int lifes{ 0 };

		void Release();
		void SetPathInfo(float _ex, float _ey);

		orcs GetType()const;
		int GetNumber() const;
		int GetMaxLifes() const;
		int GetFrame();
		int Attack();

		void Move(BAG<ASSETS>& obstacles, float gear);

		friend TDHELPER_API ORCS* OrcFactory(orcs what, float sx, float sy);
	};

	//FUNCTIONS *****************************************************

	TDHELPER_API float Distance(FPOINT first_center, FPOINT second_center);

	TDHELPER_API bool Intersect(FPOINT first_center, FPOINT second_center, float first_radius_x, float second_radius_x,
		float first_radius_y, float second_radius_y);

	TDHELPER_API bool Intersect(FRECT firstR, FRECT secondR);
	
	template<primes T>bool Sort(BAG<T>& Mesh, bool ascending = true)
	{
		if (Mesh.empty() || Mesh.size() < 2)return false;

		bool sorted{ false };

		if (ascending)
		{
			while (!sorted)
			{
				sorted = true;

				for (size_t i = 0; i < Mesh.size() - 1; ++i)
				{
					if (Mesh[i] > Mesh[i + 1])
					{
						T temp = Mesh[i];
						Mesh[i] = Mesh[i + 1];
						Mesh[i + 1] = temp;
						sorted = false;
					}
				}
			}
		}
		else
		{
			while (!sorted)
			{
				sorted = true;

				for (size_t i = 0; i < Mesh.size() - 1; ++i)
				{
					if (Mesh[i] < Mesh[i + 1])
					{
						T temp = Mesh[i];
						Mesh[i] = Mesh[i + 1];
						Mesh[i + 1] = temp;
						sorted = false;
					}
				}
			}
		}

		return true;
	}

	TDHELPER_API bool Sort(BAG<FPOINT>& Mesh, FPOINT ref_point);

	TDHELPER_API states OrcAI(ORCS& myself, BAG<FPOINT>& buildings, BAG<ORCS>& orcs);

	TDHELPER_API FPOINT TowerAI(BUILDINGS* myself, BAG<FPOINT>& orcs);

	TDHELPER_API ASSETS* AssetFactory(assets what_type, float start_x, float start_y);
	
	TDHELPER_API SHOTS* ShotFactory(shots what, float where_x, float where_y,
		float to_where_x, float to_where_y, int shot_modifier);

	TDHELPER_API BUILDINGS* BuildingFactory(buildings what, float sx, float sy);

	TDHELPER_API ORCS* OrcFactory(orcs what, float sx, float sy);
}