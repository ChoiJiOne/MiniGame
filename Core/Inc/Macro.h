#pragma once

/** 이 매크로가 정의되어 있지 않은 타입은 반드시 명시적으로 복사 생성자와 대입 연산자를 복사 생성자를 정의해야 합니다. */
#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)    \
TypeName(TypeName&&) = delete;                \
TypeName(const TypeName&) = delete;           \
TypeName& operator=(TypeName&&) = delete;     \
TypeName& operator=(const TypeName) = delete;
#endif