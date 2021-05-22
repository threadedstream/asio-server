#pragma once


class non_copyable
{
public:
	non_copyable() = default;
	non_copyable(const non_copyable& other) = delete;
	non_copyable(const non_copyable&& other) = delete;
	non_copyable& operator=(const non_copyable& other) = delete;

	virtual ~non_copyable() {};
};