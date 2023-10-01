#pragma once
#include <fftw3.h>
#include <optional>

class FFTPlan
{
    fftw_plan Plan;
public:
    explicit FFTPlan(ssize_t len, void *input, void *output)
    : Plan{fftw_plan_dft_1d(len, reinterpret_cast<fftw_complex *>(input), reinterpret_cast<fftw_complex *>(output), FFTW_FORWARD, FFTW_ESTIMATE)}
    {}
    FFTPlan(const FFTPlan &) = delete;
    FFTPlan(FFTPlan &&) = delete;
    ~FFTPlan() noexcept
    {
        if (Plan)
            fftw_destroy_plan(Plan);
    }
    void execute() { fftw_execute(Plan); }
};

template <typename T> requires (sizeof(T) == sizeof(fftw_complex))
class _fftw_alloc_stdvec
{
public:
  using value_type = T;

  constexpr _fftw_alloc_stdvec() noexcept {}

  template <typename U>
  constexpr _fftw_alloc_stdvec(const _fftw_alloc_stdvec<U> &) noexcept {}

  [[nodiscard]] T *allocate(std::size_t n)
  {
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
      throw std::bad_array_new_length{};
    if (auto p = reinterpret_cast<T *>(fftw_malloc(sizeof(T) * n)))
      return p;
    throw std::bad_alloc{};
  }

  void deallocate(T *p, std::size_t) noexcept { fftw_free(p); }
};
/*
class WelchCalculator
{
private:
  fftw_plan CurrentPlan;
  double *CurrentIn;
  std::complex<double>* CurrentOut;

  std::complex<double> *TotalOut;
  ssize_t TotalLen;
  size_t WindowSize;
  size_t NOverlap;


public:
  constexpr size_t NWindows() const { return (TotalLen - NOverlap) / (WindowSize - NOverlap); }


  ~WelchCalculator() noexcept
  {
    if (CurrentPlan)
      fftw_destroy_plan(CurrentPlan);
    if (CurrentOut)
      fftw_free(CurrentOut);
  }
  explicit WelchCalculator(double *arrayin, ssize_t ntotal, size_t nwinsize, std::optional<size_t> noverlap = std::nullopt)
      : CurrentIn{arrayin}, TotalLen{ntotal}, WindowSize{nwinsize}, NOverlap{noverlap.value_or(nwinsize / 2)}
  {
    CurrentOut = reinterpret_cast<std::complex<double>*>(fftw_alloc_complex((nwinsize+1)/2));
    CurrentPlan = fftw_plan_dft_r2c_1d(WindowSize,arrayin,reinterpret_cast<fftw_complex*>(CurrentOut),FFTW_ESTIMATE);
  }

  void DoModePSD()
  {
    for (int i = 0; i< NWindows();i++)
    {

    }
  }

  void DoModeMagnitude()
  {

  }
};
*/