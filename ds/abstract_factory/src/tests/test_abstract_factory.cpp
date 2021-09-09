#include <gtest/gtest.h>

#include <cstring>
#include <typeindex>

#include "abstract_factory.hpp"

using namespace obps;

class AbstractFactoryTest : public testing::Test
{
public:
////////////
    class Base
    {
    public:
        Base(const char * name) : m_name(name) {}
        virtual ~Base() {};

        virtual void DoStuff(char * buf) = 0;

    protected:
        const char* m_name;
    };

    class D1 final : public Base 
    {
    public:
        D1(const char * name) : Base(name) {}
        virtual ~D1() {}

        virtual void DoStuff(char * buf) override {
            std::strcpy(buf, m_name);
        }
    private:
    };

    class D2 final : public Base 
    {
    public:
        D2(const char * name) : Base(name) {}
        virtual ~D2() {}

        virtual void DoStuff(char * buf) override {
            std::strcpy(buf, "Mazal tov!");
        }
    private:
    };
/////////////
    void SetUp() override
    {
        factory.Add("Jarred", []  { return std::make_unique<D1>("Jarred"); });
        factory.Add("Carry", []  { return std::make_unique<D2>("Carry");});

        factory_type.Add(typeid(D1), []  { return std::make_unique<D1>("Jarred"); });
        factory_type.Add(typeid(D2), []  { return std::make_unique<D2>("Carry");});
    }

protected:
    AbstractFactory<std::string, Base, const char*> factory;
    AbstractFactory<std::type_index, Base, const char*> factory_type;
};


TEST_F(AbstractFactoryTest, SimpleTest)
{
    auto obj1 = factory.Create("Jarred");
    auto obj2 = factory.Create("Carry");

    char buf1[64] = {0}, buf2[64] = {0};

    obj1->DoStuff(buf1);
    obj2->DoStuff(buf2);

    ASSERT_STREQ(buf1, "Jarred");
    ASSERT_STREQ(buf2, "Mazal tov!");
}


TEST_F(AbstractFactoryTest, NoKeyTest)
{
    EXPECT_THROW(factory.Create("Kenny"), std::out_of_range);
}


TEST_F(AbstractFactoryTest, SharedPointer)
{
    std::shared_ptr<Base> b = factory.Create("Jarred");
}


TEST_F(AbstractFactoryTest, TypeInfoKey)
{
    std::shared_ptr<Base> jarred = factory_type.Create(typeid(D1));

    char buff[64] = {0};
    jarred->DoStuff(buff);

    ASSERT_STREQ(buff, "Jarred");
}
