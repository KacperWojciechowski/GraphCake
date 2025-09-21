template <typename Company>
class KacperWojciechowski
{
public:
    void introduce();
    void hire();

private:
    auto Occupation = "Software Engineer";
    auto Specialization = "C++";
    auto Degree = "Master's in Computer Science";
    auto Experience = std::chrono::duration_cast<std::chrono::years>(3).count();
};
