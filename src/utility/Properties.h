#pragma once

#define DECLARE_PROPERTY(type, name) \
Q_PROPERTY(type name MEMBER m_##name);\
type m_##name

