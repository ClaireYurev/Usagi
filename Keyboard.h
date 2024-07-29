#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window; // Exposes private non-client functions to Window
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event()
			:
			type(Type::Invalid),
			code(0u)
		{}
		Event(Type type, unsigned char code) noexcept
			:
			type(type),
			code(code)
		{}
		bool IsPress() const noexcept
		{
			return type == Type::Press;
		}
		bool IsRelease() const noexcept
		{
			return type == Type::Release;
		}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return code;
		}
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	// Key event functionality
	bool IsKeyPressed(unsigned char keycode) const noexcept; // Is a specific key currently being pressed?
	Event ReadKey() noexcept; // Pull the event off the event queue
	bool IsKeyEmpty() const noexcept; // Is there any event in the event queue?
	void ClearKey() noexcept; // Clear the event queue
	// Character event logic (not reading any key states here), used for text input (a stream of chars)
	char ReadChar() noexcept;
	bool IsCharEmpty() const noexcept;
	void ClearChar() noexcept;
	void ClearBothKeyAndChar() noexcept;
	// Autorepeat functionality
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() const noexcept;
private:
	// Used by Window; the client cannot use them
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept; // Clears the bitset that holds all the keystates
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};