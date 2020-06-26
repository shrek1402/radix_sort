// xutility internal header
#pragma once
#ifndef _XUTILITY_
#define _XUTILITY_
#ifndef RC_INVOKED
#include <yvals.h>

#include <climits>
#include <cstdlib>
#include <cstring>
#include <utility>

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

#if (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_CEE_PURE)
_EXTERN_C
// The "noalias" attribute tells the compiler optimizer that pointers going into
// these hand-vectorized algorithms won't be stored beyond the lifetime of the
// function, and that the function will only reference arrays denoted by those
// pointers. The optimizer also assumes in that case that a pointer parameter is
// not returned to the caller via the return value, so functions using "noalias"
// must usually return void. This attribute is valuable because these functions
// are in native code objects that the compiler cannot analyze. In the absence
// of the noalias attribute, the compiler has to assume that the denoted arrays
// are "globally address taken", and that any later calls to unanalyzable
// routines may modify those arrays.
__declspec(noalias) void __cdecl __std_reverse_trivially_swappable_1(
    void* _First, void* _Last) noexcept;
__declspec(noalias) void __cdecl __std_reverse_trivially_swappable_2(
    void* _First, void* _Last) noexcept;
__declspec(noalias) void __cdecl __std_reverse_trivially_swappable_4(
    void* _First, void* _Last) noexcept;
__declspec(noalias) void __cdecl __std_reverse_trivially_swappable_8(
    void* _First, void* _Last) noexcept;
_END_EXTERN_C
#endif  // (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_CEE_PURE)

_STD_BEGIN
// FUNCTION TEMPLATE _Pass_fn
// TRANSITION, VSO#386225
template <class _Fx>
struct _Ref_fn {  // pass function object by value as a reference
  template <class... _Args>
  constexpr decltype(auto) operator()(
      _Args&&... _Vals) {  // forward function call operator
    return _Fn(_STD forward<_Args>(_Vals)...);
  }

  _Fx& _Fn;
};

template <class _Fn>
_INLINE_VAR constexpr bool _Pass_functor_by_value_v =
    sizeof(_Fn) <= sizeof(void*) &&
    conjunction_v<is_trivially_copy_constructible<_Fn>,
                  is_trivially_destructible<_Fn>>;

template <class _Fn, enable_if_t<_Pass_functor_by_value_v<_Fn>, int> =
                         0>         // TRANSITION, if constexpr
constexpr _Fn _Pass_fn(_Fn _Val) {  // pass functor by value
  return _Val;
}

template <class _Fn, enable_if_t<!_Pass_functor_by_value_v<_Fn>, int> = 0>
constexpr _Ref_fn<_Fn> _Pass_fn(_Fn& _Val) {  // pass functor by "reference"
  return {_Val};
}

// ITERATOR STUFF (from <iterator>)
// ITERATOR TAGS (from <iterator>)
struct input_iterator_tag {  // identifying tag for input iterators
};

struct output_iterator_tag {  // identifying tag for output iterators
};

struct forward_iterator_tag
    : input_iterator_tag {  // identifying tag for forward iterators
};

struct bidirectional_iterator_tag
    : forward_iterator_tag {  // identifying tag for bidirectional iterators
};

struct random_access_iterator_tag
    : bidirectional_iterator_tag {  // identifying tag for random-access
                                    // iterators
};

// STRUCT _Unused_parameter
struct _Unused_parameter {  // generic unused parameter struct
  constexpr _Unused_parameter() noexcept = default;
  template <class _Ty>
  constexpr _Unused_parameter(_Ty&&) noexcept {}
};

// ALIAS _Any_tag
using _Any_tag = _Unused_parameter;  // generic fallback/default/"other" target
                                     // for tag dispatch

// ALIAS TEMPLATE _Algorithm_int_t
template <class _Ty>
using _Algorithm_int_t = conditional_t<is_integral_v<_Ty>, _Ty, ptrdiff_t>;

// STRUCT TEMPLATE iterator_traits
template <class,
          class = void>
struct _Iterator_traits_base {  // empty for non-iterators
};

template <class _Iter>
struct _Iterator_traits_base<
    _Iter,
    void_t<typename _Iter::iterator_category, typename _Iter::value_type,
           typename _Iter::difference_type, typename _Iter::pointer,
           typename _Iter::reference>> {  // defined if _Iter::* types exist
  using iterator_category = typename _Iter::iterator_category;
  using value_type = typename _Iter::value_type;
  using difference_type = typename _Iter::difference_type;

  using pointer = typename _Iter::pointer;
  using reference = typename _Iter::reference;
};

template <class _Ty,
          bool = is_object_v<_Ty>>
struct _Iterator_traits_pointer_base {  // iterator properties for pointers to
                                        // object
  using iterator_category = random_access_iterator_tag;
  using value_type = remove_cv_t<_Ty>;
  using difference_type = ptrdiff_t;

  using pointer = _Ty*;
  using reference = _Ty&;
};

template <class _Ty>
struct _Iterator_traits_pointer_base<
    _Ty, false> {  // iterator properties for pointers to non-object
                   // not actually iterators, as no arithmetic is possible -> no
                   // members
};

template <class _Iter>
struct iterator_traits
    : _Iterator_traits_base<_Iter> {  // get traits from iterator _Iter, if
                                      // possible
};

template <class _Ty>
struct iterator_traits<_Ty*>
    : _Iterator_traits_pointer_base<_Ty> {  // get traits from pointer, if
                                            // possible
};

// ALIAS TEMPLATE _Iter_ref_t
template <class _Iter>
using _Iter_ref_t = typename iterator_traits<_Iter>::reference;

// ALIAS TEMPLATE _Iter_value_t
template <class _Iter>
using _Iter_value_t = typename iterator_traits<_Iter>::value_type;

// ALIAS TEMPLATE _Iter_diff_t
template <class _Iter>
using _Iter_diff_t = typename iterator_traits<_Iter>::difference_type;

// ALIAS TEMPLATE _Common_diff_t
template <class... _Iters>
using _Common_diff_t = common_type_t<_Iter_diff_t<_Iters>...>;

// ALIAS TEMPLATE _Iter_cat_t
template <class _Iter>
using _Iter_cat_t = typename iterator_traits<_Iter>::iterator_category;

// VARIABLE TEMPLATE _Is_iterator_v
template <class _Ty, class = void>
_INLINE_VAR constexpr bool _Is_iterator_v = false;

template <class _Ty>
_INLINE_VAR constexpr bool _Is_iterator_v<_Ty, void_t<_Iter_cat_t<_Ty>>> = true;

// TRAIT _Is_iterator
template <class _Ty>
struct _Is_iterator : bool_constant<_Is_iterator_v<_Ty>> {};

// VARIABLE TEMPLATE _Is_input_iter_v
template <class _Iter>
_INLINE_VAR constexpr bool _Is_input_iter_v =
    is_convertible_v<_Iter_cat_t<_Iter>, input_iterator_tag>;

// VARIABLE TEMPLATE _Is_fwd_iter_v
template <class _Iter>
_INLINE_VAR constexpr bool _Is_fwd_iter_v =
    is_convertible_v<_Iter_cat_t<_Iter>, forward_iterator_tag>;

// VARIABLE TEMPLATE _Is_bidi_iter_v
template <class _Iter>
_INLINE_VAR constexpr bool _Is_bidi_iter_v =
    is_convertible_v<_Iter_cat_t<_Iter>, bidirectional_iterator_tag>;

// VARIABLE TEMPLATE _Is_random_iter_v
template <class _Iter>
_INLINE_VAR constexpr bool _Is_random_iter_v =
    is_convertible_v<_Iter_cat_t<_Iter>, random_access_iterator_tag>;

// STRUCT TEMPLATE _Is_checked_helper
template <class,
          class = void>
struct _Is_checked_helper {  // default definition, no longer used, retained due
                             // to pseudo-documentation
};

// FUNCTION TEMPLATE _Adl_verify_range
#if _ITERATOR_DEBUG_LEVEL != 0
template <class _Ty>
constexpr void _Verify_range(
    const _Ty* const _First,
    const _Ty* const
        _Last) noexcept {  // special case range verification for pointers
  _STL_VERIFY(_First <= _Last, "transposed pointer range");
}
#endif  // _ITERATOR_DEBUG_LEVEL != 0

template <class _Iter, class = void>
struct _Allow_inheriting_unwrap : true_type {};

template <class _Iter>
struct _Allow_inheriting_unwrap<
    _Iter,
    enable_if_t<!is_same_v<_Iter, typename _Iter::_Prevent_inheriting_unwrap>>>
    : false_type {};

template <class _Iter, class _Sentinel = _Iter, class = void>
struct _Range_verifiable : false_type {};

template <class _Iter, class _Sentinel>
struct _Range_verifiable<
    _Iter, _Sentinel,
    void_t<decltype(_Verify_range(_STD declval<const _Iter&>(),
                                  _STD declval<const _Sentinel&>()))>>
    : _Allow_inheriting_unwrap<_Iter>::type {};

template <class _Iter, class _Sentinel = _Iter>
_INLINE_VAR constexpr bool _Range_verifiable_v =
    _Range_verifiable<_Iter, _Sentinel>::value;

#if _HAS_IF_CONSTEXPR
template <class _Iter, class _Sentinel>
constexpr void _Adl_verify_range(
    const _Iter& _First,
    const _Sentinel&
        _Last) {  // check that [_First, _Last) forms an iterator range
  if constexpr (_Range_verifiable_v<_Iter, _Sentinel>) {
    _Verify_range(_First, _Last);
  } else {
    (void)_First;  // TRANSITION, VSO#486357
    (void)_Last;   // TRANSITION, VSO#486357
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _Iter, class _Sentinel>
constexpr void _Adl_verify_range1(
    const _Iter& _First, const _Sentinel& _Last,
    true_type) {  // check that [_First, _Last) forms an iterator range
  _Verify_range(_First, _Last);
}

template <class _Iter, class _Sentinel>
constexpr void _Adl_verify_range1(
    const _Iter&, const _Sentinel&,
    false_type) {  // (don't) check that [_First, _Last) forms an iterator range
}

template <class _Iter, class _Sentinel>
constexpr void _Adl_verify_range(
    const _Iter& _First,
    const _Sentinel&
        _Last) {  // check that [_First, _Last) forms an iterator range
  _Adl_verify_range1(_First, _Last,
                     bool_constant<_Range_verifiable_v<_Iter, _Sentinel>>{});
}
#endif  // _HAS_IF_CONSTEXPR

// FUNCTION TEMPLATE _Get_unwrapped
template <class _Iter, class = void>
struct _Unwrappable : false_type {};

template <class _Iter>
struct _Unwrappable<_Iter, void_t<decltype(_STD declval<_Iter&>()._Seek_to(
                               _STD declval<const _Iter&>()._Unwrapped()))>>
    : _Allow_inheriting_unwrap<_Iter>::type {};

template <class _Iter>
_INLINE_VAR constexpr bool _Unwrappable_v = _Unwrappable<_Iter>::value;

template <class _Iter, enable_if_t<_Unwrappable_v<_Iter>, int> = 0>
_NODISCARD constexpr auto _Get_unwrapped(const _Iter& _It) {
  // unwrap an iterator previously subjected to _Adl_verify_range or otherwise
  // validated
  return _It._Unwrapped();
}

template <class _Iter, enable_if_t<!_Unwrappable_v<_Iter>, int> = 0>
_NODISCARD constexpr const _Iter& _Get_unwrapped(const _Iter& _It) {
  // (don't) unwrap an iterator previously subjected to _Adl_verify_range or
  // otherwise validated
  return _It;
}

template <class _Iter, enable_if_t<!_Unwrappable_v<_Iter>, int> = 0>
_NODISCARD constexpr const _Iter&& _Get_unwrapped(const _Iter&& _It) {
  // (don't) unwrap an iterator previously subjected to _Adl_verify_range or
  // otherwise validated
  return static_cast<const _Iter&&>(_It);
}

template <class _Ty>
_NODISCARD constexpr _Ty* _Get_unwrapped(
    _Ty* const _Ptr) {  // special case already-unwrapped pointers
  return _Ptr;
}

template <class _Iter>
using _Unwrapped_t =
    _Remove_cvref_t<decltype(_Get_unwrapped(_STD declval<const _Iter&>()))>;

// FUNCTION TEMPLATE _Get_unwrapped_unverified
template <class _Iter, class = bool>
struct _Do_unwrap_when_unverified : false_type {};

template <class _Iter>
struct _Do_unwrap_when_unverified<_Iter, decltype(static_cast<bool>(
                                             _Iter::_Unwrap_when_unverified))>
    : bool_constant<static_cast<bool>(_Iter::_Unwrap_when_unverified)> {};

template <class _Iter>
_INLINE_VAR constexpr bool _Do_unwrap_when_unverified_v =
    _Do_unwrap_when_unverified<_Iter>::value;

template <class _Iter>
_INLINE_VAR constexpr bool _Unwrappable_for_unverified_v =
    _Unwrappable_v<_Iter>&& _Do_unwrap_when_unverified_v<_Iter>;

template <class _Iter,
          enable_if_t<_Unwrappable_for_unverified_v<_Iter>, int> = 0>
_NODISCARD constexpr auto _Get_unwrapped_unverified(const _Iter& _It) {
  // unwrap an iterator not previously subjected to _Adl_verify_range
  return _It._Unwrapped();
}

template <class _Iter,
          enable_if_t<!_Unwrappable_for_unverified_v<_Iter>, int> = 0>
_NODISCARD constexpr const _Iter& _Get_unwrapped_unverified(const _Iter& _It) {
  // (don't) unwrap an iterator not previously subjected to _Adl_verify_range
  return _It;
}

template <class _Iter,
          enable_if_t<!_Unwrappable_for_unverified_v<_Iter>, int> = 0>
_NODISCARD constexpr const _Iter&& _Get_unwrapped_unverified(
    const _Iter&& _It) {
  // (don't) unwrap an iterator not previously subjected to _Adl_verify_range
  return static_cast<const _Iter&&>(_It);
}

template <class _Ty>
_NODISCARD constexpr _Ty* _Get_unwrapped_unverified(
    _Ty* const _Ptr) {  // special case already-unwrapped pointers
  return _Ptr;
}

template <class _Iter>
using _Unwrapped_unverified_t = _Remove_cvref_t<decltype(
    _Get_unwrapped_unverified(_STD declval<const _Iter&>()))>;

// FUNCTION TEMPLATE _Get_unwrapped_n
struct _Distance_unknown {
  constexpr _Distance_unknown operator-() const noexcept { return {}; }
};

template <class _Diff>
_INLINE_VAR constexpr _Diff _Max_possible_v =
    static_cast<_Diff>(static_cast<make_unsigned_t<_Diff>>(-1) >> 1);

template <class _Diff>
_INLINE_VAR constexpr _Diff _Min_possible_v = -_Max_possible_v<_Diff> - 1;

template <class _Iter, class = void>
struct _Offset_verifiable : false_type {};

template <class _Iter>
struct _Offset_verifiable<
    _Iter, void_t<decltype(_STD declval<const _Iter&>()._Verify_offset(
               _Iter_diff_t<_Iter>{}))>> : true_type {};

template <class _Iter>
_INLINE_VAR constexpr bool _Offset_verifiable_v =
    _Offset_verifiable<_Iter>::value;

template <class _Iter>
_INLINE_VAR constexpr bool _Unwrappable_for_offset_v =
    _Unwrappable_v<_Iter>&& _Offset_verifiable_v<_Iter>;

template <class _Iter, class _Diff,
          enable_if_t<_Unwrappable_for_offset_v<_Iter> && is_integral_v<_Diff>,
                      int> = 0>
_NODISCARD constexpr auto _Get_unwrapped_n(const _Iter& _It, const _Diff _Off) {
  // ask an iterator to assert that the iterator moved _Off positions is valid,
  // and unwrap
  using _IDiff = _Iter_diff_t<_Iter>;
  using _CDiff = common_type_t<_Diff, _IDiff>;
  const auto _COff = static_cast<_CDiff>(_Off);

  _STL_ASSERT(_COff <= static_cast<_CDiff>(_Max_possible_v<_IDiff>) &&
                  (is_unsigned_v<_Diff> ||
                   static_cast<_CDiff>(_Min_possible_v<_IDiff>) <= _COff),
              "integer overflow");
  (void)_COff;

  _It._Verify_offset(static_cast<_IDiff>(_Off));
  return _It._Unwrapped();
}

template <class _Iter, class _Diff,
          enable_if_t<_Unwrappable_for_unverified_v<_Iter>  //
                          && ((!_Unwrappable_for_offset_v<_Iter> &&
                               is_integral_v<_Diff>) ||
                              is_same_v<_Diff, _Distance_unknown>),
                      int> = 0>
_NODISCARD constexpr auto _Get_unwrapped_n(const _Iter& _It, _Diff) {
  // iterator doesn't support offset-based asserts, or offset unknown; defer to
  // unverified unwrap
  return _It._Unwrapped();
}

template <class _Iter, class _Diff,
          enable_if_t<!_Unwrappable_for_unverified_v<_Iter>  //
                          && ((!_Unwrappable_for_offset_v<_Iter> &&
                               is_integral_v<_Diff>) ||
                              is_same_v<_Diff, _Distance_unknown>),
                      int> = 0>
_NODISCARD constexpr const _Iter& _Get_unwrapped_n(const _Iter& _It, _Diff) {
  // pass through lvalue iterator that doesn't participate in checking
  return _It;
}

template <class _Iter, class _Diff,
          enable_if_t<!_Unwrappable_for_unverified_v<_Iter>  //
                          && ((!_Unwrappable_for_offset_v<_Iter> &&
                               is_integral_v<_Diff>) ||
                              is_same_v<_Diff, _Distance_unknown>),
                      int> = 0>
_NODISCARD constexpr const _Iter&& _Get_unwrapped_n(const _Iter&& _It, _Diff) {
  // pass through rvalue iterator that doesn't participate in checking
  return static_cast<const _Iter&&>(_It);
}

template <
    class _Ty, class _Diff,
    enable_if_t<is_same_v<_Diff, _Distance_unknown> || is_integral_v<_Diff>,
                int> = 0>
_NODISCARD constexpr _Ty* _Get_unwrapped_n(_Ty* const _Src, _Diff) {
  return _Src;
}

template <class _Iter>
using _Unwrapped_n_t = _Remove_cvref_t<decltype(
    _Get_unwrapped_n(_STD declval<const _Iter&>(), _Iter_diff_t<_Iter>{}))>;

// FUNCTION TEMPLATE _Seek_wrapped
template <class _Iter, class _UIter, class = void>
struct _Wrapped_seekable : false_type {};

template <class _Iter, class _UIter>
struct _Wrapped_seekable<_Iter, _UIter,
                         void_t<decltype(_STD declval<_Iter&>()._Seek_to(
                             _STD declval<const _UIter&>()))>> : true_type {};

template <class _Iter, class _UIter>
_INLINE_VAR constexpr bool _Wrapped_seekable_v =
    _Wrapped_seekable<_Iter, _UIter>::value;

#if _HAS_IF_CONSTEXPR
template <class _Iter, class _UIter>
constexpr void _Seek_wrapped(_Iter& _It, const _UIter& _UIt) {
  if constexpr (_Wrapped_seekable_v<_Iter, _UIter>) {
    _It._Seek_to(_UIt);
  } else {
    _It = _UIt;
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _Iter, class _UIter,
          enable_if_t<_Wrapped_seekable_v<_Iter, _UIter>, int> = 0>
constexpr void _Seek_wrapped(_Iter& _It, const _UIter& _UIt) {
  _It._Seek_to(_UIt);
}

template <class _Iter, class _UIter,
          enable_if_t<!_Wrapped_seekable_v<_Iter, _UIter>, int> = 0>
constexpr void _Seek_wrapped(_Iter& _It, const _UIter& _UIt) {
  _It = _UIt;
}
#endif  // _HAS_IF_CONSTEXPR

template <class _Ty>
constexpr void _Seek_wrapped(_Ty*& _It, _Ty* const _UIt) {
  _It = _UIt;
}

#if _HAS_CXX17
// STRUCT TEMPLATE _Is_allocator
template <class _Ty, class = void>
struct _Is_allocator
    : false_type {  // selected when _Ty can't possibly be an allocator
};

template <class _Ty>
struct _Is_allocator<
    _Ty, void_t<typename _Ty::value_type,
                decltype(_STD declval<_Ty&>().deallocate(
                    _STD declval<_Ty&>().allocate(size_t{1}), size_t{1}))>>
    : true_type {  // selected when _Ty resembles an allocator, N4687 26.2.1
                   // [container.requirements.general]/17
};

// ALIAS TEMPLATES FOR DEDUCTION GUIDES, N4687 26.4.1 [associative.general]/2
template <class _Iter>
using _Guide_key_t =
    remove_const_t<typename iterator_traits<_Iter>::value_type::first_type>;

template <class _Iter>
using _Guide_val_t = typename iterator_traits<_Iter>::value_type::second_type;

template <class _Iter>
using _Guide_pair_t =
    pair<add_const_t<typename iterator_traits<_Iter>::value_type::first_type>,
         typename iterator_traits<_Iter>::value_type::second_type>;

// STRUCT TEMPLATE is_execution_policy
template <class _Ty>
struct is_execution_policy : false_type {};

template <class _Ty>
inline constexpr bool is_execution_policy_v = is_execution_policy<_Ty>::value;

// ALIAS TEMPLATE _Enable_if_execution_policy_t
// Note: The noexcept specifiers on all parallel algorithm overloads enforce
// termination as per N4713 23.19.4 [execpol.seq]/2, 23.19.5 [execpol.par]/2,
// and 23.19.6 [execpol.parunseq]/2
template <class _ExPo>
using _Enable_if_execution_policy_t =
    typename remove_reference_t<_ExPo>::_Standard_execution_policy;

#define _REQUIRE_PARALLEL_ITERATOR(_Iter) \
  static_assert(_Is_fwd_iter_v<_Iter>,    \
                "Parallel algorithms require forward iterators or stronger.")

#endif  // _HAS_CXX17

// FUNCTION TEMPLATE _Idl_distance
#if _HAS_IF_CONSTEXPR
template <class _Checked, class _Iter>
inline auto _Idl_distance(const _Iter& _First, const _Iter& _Last) {
  // tries to get the distance between _First and _Last if they are
  // random-access iterators
  if constexpr (_Is_random_iter_v<_Iter>) {
    return static_cast<_Iter_diff_t<_Checked>>(_Last - _First);
  } else {
    (void)_First;  // TRANSITION, VSO#486357
    (void)_Last;   // TRANSITION, VSO#486357
    return _Distance_unknown{};
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _Checked, class _Iter>
inline _Distance_unknown _Idl_distance1(const _Iter&, const _Iter&,
                                        input_iterator_tag) {
  // _Idl_distance for non-random-access iterators
  return {};
}

template <class _Checked, class _Iter>
inline _Iter_diff_t<_Checked> _Idl_distance1(const _Iter& _First,
                                             const _Iter& _Last,
                                             random_access_iterator_tag) {
  // _Idl_distance for random-access iterators
  return static_cast<_Iter_diff_t<_Checked>>(_Last - _First);
}

template <class _Checked, class _Iter>
inline auto _Idl_distance(const _Iter& _First, const _Iter& _Last) {
  // tries to get the distance between _First and _Last if they are
  // random-access iterators
  return _Idl_distance1<_Checked>(_First, _Last, _Iter_cat_t<_Iter>());
}
#endif  // _HAS_IF_CONSTEXPR

// STRUCT TEMPLATE _Unwrap_enum AND ALIAS
template <class _Elem, bool _Is_enum = is_enum_v<_Elem>>
struct _Unwrap_enum {  // if _Elem is an enum, gets its underlying type;
                       // otherwise leaves _Elem unchanged
  using type = underlying_type_t<_Elem>;
};

template <class _Elem>
struct _Unwrap_enum<_Elem, false> {  // passthrough non-enum type
  using type = _Elem;
};

template <class _Elem>
using _Unwrap_enum_t = typename _Unwrap_enum<_Elem>::type;

// ALGORITHM DISPATCH TRAITS
template <class>
class move_iterator;

template <class _Source, class _Dest>
struct _Ptr_cat_helper {  // determines _Ptr_cat's result in the most general
                          // case
  using _USource = _Unwrap_enum_t<_Source>;
  using _UDest = _Unwrap_enum_t<_Dest>;
  static constexpr bool _Really_trivial = conjunction_v<
      bool_constant<sizeof(_USource) == sizeof(_UDest) &&
                    is_same_v<bool, _USource> == is_same_v<bool, _UDest>>,
      is_integral<_USource>, is_integral<_UDest>>;
  static constexpr bool _Trivially_copyable = _Really_trivial;
};

template <class _Elem>
struct _Ptr_cat_helper<_Elem, _Elem> {  // determines _Ptr_cat's result when the
                                        // types are the same
  static constexpr bool _Really_trivial =
      conjunction_v<is_trivial<_Elem>, is_trivially_copyable<_Elem>>;
  static constexpr bool _Trivially_copyable = is_trivially_copyable_v<_Elem>;
};

template <class _Anything>
struct _Ptr_cat_helper<_Anything*, const _Anything*> {
  // determines _Ptr_cat's result when all we do is add const to a pointer
  static constexpr bool _Really_trivial = true;
  static constexpr bool _Trivially_copyable = true;
};

template <class _Anything>
struct _Ptr_cat_helper<_Anything*, volatile _Anything*> {
  // determines _Ptr_cat's result when all we do is add volatile to a pointer
  static constexpr bool _Really_trivial = true;
  static constexpr bool _Trivially_copyable = true;
};

template <class _Anything>
struct _Ptr_cat_helper<_Anything*, const volatile _Anything*> {
  // determines _Ptr_cat's result when all we do is add cv to a pointer
  static constexpr bool _Really_trivial = true;
  static constexpr bool _Trivially_copyable = true;
};

struct _False_copy_cat {
  static constexpr bool _Really_trivial = false;
  static constexpr bool _Trivially_copyable = false;
};

template <class _Source, class _Dest>
struct _Ptr_copy_cat : _False_copy_cat {
};  // return pointer copy optimization category for arbitrary iterators

template <class _Source, class _Dest>
struct _Ptr_copy_cat<_Source*, _Dest*>
    : conditional_t<is_trivially_assignable_v<_Dest&, _Source&>,
                    _Ptr_cat_helper<remove_cv_t<_Source>, remove_cv_t<_Dest>>,
                    _False_copy_cat> {
};  // return pointer copy optimization category for pointers

template <class _Source, class _Dest>
struct _Ptr_copy_cat<move_iterator<_Source*>, _Dest*>
    : _Ptr_copy_cat<_Source*, _Dest*> {
};  // return pointer copy optimization category for move iterators

template <class _Source, class _Dest>
struct _Ptr_move_cat : _False_copy_cat {
};  // return pointer move optimization category for arbitrary iterators

template <class _Source, class _Dest>
struct _Ptr_move_cat<_Source*, _Dest*>
    : conditional_t<is_trivially_assignable_v<_Dest&, _Source>,
                    _Ptr_cat_helper<remove_cv_t<_Source>, remove_cv_t<_Dest>>,
                    _False_copy_cat> {
};  // return pointer move optimization category for pointers

template <class _Source, class _Dest>
struct _Ptr_move_cat<move_iterator<_Source*>, _Dest*>
    : _Ptr_move_cat<_Source*, _Dest*> {
};  // return pointer move optimization category for move iterators

// DEBUG TESTING MACROS

#if _ITERATOR_DEBUG_LEVEL < 2
#define _DEBUG_LT_PRED(pred, x, y) static_cast<bool>(pred(x, y))
#define _DEBUG_ORDER_UNWRAPPED(first, last, pred)
#define _DEBUG_ORDER_SET_UNWRAPPED(otherIter, first, last, pred)

#else  // _ITERATOR_DEBUG_LEVEL < 2
#define _DEBUG_LT_PRED(pred, x, y) _Debug_lt_pred(pred, x, y)
#define _DEBUG_ORDER_UNWRAPPED(first, last, pred) \
  _Debug_order_unchecked(first, last, pred)
#define _DEBUG_ORDER_SET_UNWRAPPED(otherIter, first, last, pred) \
  _Debug_order_set_unchecked<otherIter>(first, last, pred)

// FUNCTION TEMPLATE _Debug_lt_pred
template <class _Pr, class _Ty1, class _Ty2,
          enable_if_t<is_same_v<_Remove_cvref_t<_Ty1>, _Remove_cvref_t<_Ty2>>,
                      int> = 0>
constexpr bool _Debug_lt_pred(_Pr&& _Pred, _Ty1&& _Left, _Ty2&& _Right)
    _NOEXCEPT_COND(noexcept(_Pred(_Left, _Right)) &&
                   noexcept(_Pred(_Right, _Left))) {
  // test if _Pred(_Left, _Right) and _Pred is strict weak ordering, when the
  // arguments are the cv-same-type
  const auto _Result = static_cast<bool>(_Pred(_Left, _Right));
  if (_Result) {
    _STL_VERIFY(!_Pred(_Right, _Left), "invalid comparator");
  }

  return _Result;
}

template <class _Pr, class _Ty1, class _Ty2,
          enable_if_t<!is_same_v<_Remove_cvref_t<_Ty1>, _Remove_cvref_t<_Ty2>>,
                      int> = 0>
constexpr bool _Debug_lt_pred(_Pr&& _Pred, _Ty1&& _Left, _Ty2&& _Right)
    _NOEXCEPT_COND(noexcept(_Pred(_Left, _Right))) {
  // test if _Pred(_Left, _Right); no debug checks as the types differ
  return static_cast<bool>(_Pred(_Left, _Right));
}

// FUNCTION TEMPLATE _Debug_order_unchecked
#if _HAS_IF_CONSTEXPR
template <class _InIt, class _Sentinel, class _Pr>
inline void _Debug_order_unchecked(_InIt _First, _Sentinel _Last, _Pr&& _Pred) {
  // test if range is ordered by predicate
  if constexpr (_Is_fwd_iter_v<_InIt>) {
    if (_First != _Last) {
      for (auto _Next = _First; ++_Next != _Last; _First = _Next) {
        _STL_VERIFY(!static_cast<bool>(_Pred(*_Next, *_First)),
                    "sequence not ordered");
      }
    }
  } else {
    (void)_First;  // TRANSITION, VSO#486357
    (void)_Last;   // TRANSITION, VSO#486357
    (void)_Pred;   // TRANSITION, VSO#486357
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _InIt, class _Sentinel, class _Pr>
inline void _Debug_order_unchecked2(_InIt, _Sentinel, _Pr&,
                                    input_iterator_tag) {
  // (don't) test if range is ordered by predicate, input iterators
}

template <class _FwdIt, class _Sentinel, class _Pr>
inline void _Debug_order_unchecked2(_FwdIt _First, _Sentinel _Last, _Pr& _Pred,
                                    forward_iterator_tag) {
  // test if range is ordered by predicate, forward iterators
  if (_First != _Last) {
    for (_FwdIt _Next = _First; ++_Next != _Last; _First = _Next) {
      _STL_VERIFY(!static_cast<bool>(_Pred(*_Next, *_First)),
                  "sequence not ordered");
    }
  }
}

template <class _InIt, class _Sentinel, class _Pr>
inline void _Debug_order_unchecked(_InIt _First, _Sentinel _Last, _Pr&& _Pred) {
  // test if range is ordered by predicate
  _Debug_order_unchecked2(_First, _Last, _Pred, _Iter_cat_t<_InIt>());
}
#endif  // _HAS_IF_CONSTEXPR

// FUNCTION TEMPLATE _Debug_order_set_unchecked
#if _HAS_IF_CONSTEXPR
template <class _OtherIt, class _InIt, class _Pr>
inline void _Debug_order_set_unchecked(_InIt _First, _InIt _Last, _Pr&& _Pred) {
  // test if range is ordered by predicate
  if constexpr (is_same_v<_Iter_value_t<_OtherIt>, _Iter_value_t<_InIt>> &&
                _Is_fwd_iter_v<_InIt>) {
    _Debug_order_unchecked(_First, _Last, _Pred);
  } else {
    (void)_First;  // TRANSITION, VSO#486357
    (void)_Last;   // TRANSITION, VSO#486357
    (void)_Pred;   // TRANSITION, VSO#486357
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <size_t _Value>
struct _Priority_tag
    : _Priority_tag<_Value - 1> {  // priority tag for tag dispatch
};

template <>
struct _Priority_tag<0> {  // base case priority tag for tag dispatch
};

template <class _InIt, class _Pr>
inline void _Debug_order_set_unchecked2(
    _InIt, _InIt, _Pr&, input_iterator_tag,
    _Priority_tag<0>) {  // (don't) test if range is ordered by predicate, input
                         // iterators or different types
}

template <class _FwdIt, class _Pr>
inline void _Debug_order_set_unchecked2(_FwdIt _First, _FwdIt _Last, _Pr& _Pred,
                                        forward_iterator_tag,
                                        _Priority_tag<1>) {
  // test if range is ordered by predicate, forward iterators and same types
  _Debug_order_unchecked2(_First, _Last, _Pred, forward_iterator_tag{});
}

template <class _OtherIt, class _InIt, class _Pr>
inline void _Debug_order_set_unchecked(_InIt _First, _InIt _Last, _Pr&& _Pred) {
  // test if range is ordered by predicate
  _Debug_order_set_unchecked2(
      _First, _Last, _Pred, _Iter_cat_t<_InIt>(),
      _Priority_tag<
          is_same<_Iter_value_t<_OtherIt>, _Iter_value_t<_InIt>>::value>());
}
#endif  // _HAS_IF_CONSTEXPR
#endif  // _ITERATOR_DEBUG_LEVEL < 2

// MORE ITERATOR STUFF (from <iterator>)
// FUNCTION TEMPLATE advance
#if _HAS_IF_CONSTEXPR
template <class _InIt, class _Diff>
_CONSTEXPR17 void advance(_InIt& _Where,
                          _Diff _Off) {  // increment iterator by offset
  if constexpr (_Is_random_iter_v<_InIt>) {
    _Where += _Off;
  } else {
    if constexpr (is_signed_v<_Diff>) {
      if constexpr (_Is_bidi_iter_v<_InIt>) {
        for (; _Off < 0; ++_Off) {
          --_Where;
        }
      } else {
        _STL_ASSERT(_Off >= 0,
                    "negative advance of non-bidirectional iterator");
      }
    }

    for (; 0 < _Off; --_Off) {
      ++_Where;
    }
  }
}
#else  // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _InIt, class _Diff>
_CONSTEXPR17 void _Advance1(_InIt& _Where, _Diff _Off, input_iterator_tag) {
  // increment iterator by offset, input iterators
  _STL_ASSERT(_Off >= 0, "negative advance of non-bidirectional iterator");
  for (; 0 < _Off; --_Off) {
    ++_Where;
  }
}

template <class _BidIt, class _Diff>
_CONSTEXPR17 void _Advance1(_BidIt& _Where, _Diff _Off,
                            bidirectional_iterator_tag) {
  // increment iterator by offset, bidirectional iterators
  for (; 0 < _Off; --_Off) {
    ++_Where;
  }

  // the following warning is triggered if _Diff is unsigned
#pragma warning( \
    suppress : 6294)  // Ill-defined for-loop: initial condition does not
                      // satisfy test. Loop body not executed.
  for (; _Off < 0; ++_Off) {
    --_Where;
  }
}

template <class _RanIt, class _Diff>
_CONSTEXPR17 void _Advance1(_RanIt& _Where, _Diff _Off,
                            random_access_iterator_tag) {
  // increment iterator by offset, random-access iterators
  _Where += _Off;
}

template <class _InIt, class _Diff>
_CONSTEXPR17 void advance(_InIt& _Where, _Diff _Off) {
  // increment iterator by offset, arbitrary iterators
  // we remove_const_t before _Iter_cat_t for better diagnostics if the user
  // passes an iterator that is const
  _Advance1(_Where, _Off, _Iter_cat_t<remove_const_t<_InIt>>());
}
#endif  // _HAS_IF_CONSTEXPR

// FUNCTION TEMPLATE distance
#if _HAS_IF_CONSTEXPR
template <class _InIt>
_NODISCARD _CONSTEXPR17 _Iter_diff_t<_InIt> distance(
    _InIt _First, _InIt _Last) {  // return distance between iterators
  if constexpr (_Is_random_iter_v<_InIt>) {
    return _Last - _First;  // assume the iterator will do debug checking
  } else {
    _Adl_verify_range(_First, _Last);
    auto _UFirst = _Get_unwrapped(_First);
    const auto _ULast = _Get_unwrapped(_Last);
    _Iter_diff_t<_InIt> _Off = 0;
    for (; _UFirst != _ULast; ++_UFirst) {
      ++_Off;
    }

    return _Off;
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _InIt>
_CONSTEXPR17 _Iter_diff_t<_InIt> _Distance1(_InIt _First, _InIt _Last,
                                            input_iterator_tag) {
  // return distance between iterators; input
  _Adl_verify_range(_First, _Last);
  auto _UFirst = _Get_unwrapped(_First);
  const auto _ULast = _Get_unwrapped(_Last);
  _Iter_diff_t<_InIt> _Off = 0;
  for (; _UFirst != _ULast; ++_UFirst) {
    ++_Off;
  }

  return _Off;
}

template <class _RanIt>
_CONSTEXPR17 _Iter_diff_t<_RanIt> _Distance1(_RanIt _First, _RanIt _Last,
                                             random_access_iterator_tag) {
  // return distance between iterators; random-access
  return _Last - _First;
}

template <class _InIt>
_NODISCARD _CONSTEXPR17 _Iter_diff_t<_InIt> distance(
    _InIt _First, _InIt _Last) {  // return distance between iterators
  return _Distance1(_First, _Last, _Iter_cat_t<_InIt>());
}
#endif  // _HAS_IF_CONSTEXPR

// FUNCTION TEMPLATE _Next_iter
template <class _InIt>
constexpr _InIt _Next_iter(_InIt _First) {  // increment iterator
  return ++_First;
}

// FUNCTION TEMPLATE next
template <class _InIt>
_NODISCARD _CONSTEXPR17 _InIt
next(_InIt _First, _Iter_diff_t<_InIt> _Off = 1) {  // increment iterator
  static_assert(_Is_input_iter_v<_InIt>, "next requires input iterator");

  _STD advance(_First, _Off);
  return _First;
}

// FUNCTION TEMPLATE _Prev_iter
template <class _BidIt>
constexpr _BidIt _Prev_iter(_BidIt _First) {  // decrement iterator
  return --_First;
}

// FUNCTION TEMPLATE prev
template <class _BidIt>
_NODISCARD _CONSTEXPR17 _BidIt
prev(_BidIt _First, _Iter_diff_t<_BidIt> _Off = 1) {  // decrement iterator
  static_assert(_Is_bidi_iter_v<_BidIt>,
                "prev requires bidirectional iterator");

  _STD advance(_First, -_Off);
  return _First;
}

// CLASS TEMPLATE reverse_iterator
template <class _Ty>
struct pointer_traits;

template <class _Iterator>
constexpr _Iterator _Operator_arrow(
    _Iterator _Target,
    true_type) {  // return operator-> where _Iterator is a pointer
  return _Target;
}

template <class _Iterator>
constexpr decltype(auto) _Operator_arrow(_Iterator&& _Target, false_type) {
  // return operator-> where _Iterator is a class type
  return _STD forward<_Iterator>(_Target).operator->();
}

template <class _BidIt>
class reverse_iterator {  // wrap iterator to run it backwards
 public:
  using iterator_category = typename iterator_traits<_BidIt>::iterator_category;
  using value_type = typename iterator_traits<_BidIt>::value_type;
  using difference_type = typename iterator_traits<_BidIt>::difference_type;
  using pointer = typename iterator_traits<_BidIt>::pointer;
  using reference = typename iterator_traits<_BidIt>::reference;

  using iterator_type = _BidIt;

  _CONSTEXPR17 reverse_iterator() : current() {}

  _CONSTEXPR17 explicit reverse_iterator(_BidIt _Right) : current(_Right) {}

  template <class _Other>
  _CONSTEXPR17 reverse_iterator(const reverse_iterator<_Other>& _Right)
      : current(_Right.base()) {}

  template <class _Other>
  _CONSTEXPR17 reverse_iterator& operator=(
      const reverse_iterator<_Other>& _Right) {
    current = _Right.base();
    return *this;
  }

  _NODISCARD _CONSTEXPR17 _BidIt base() const { return current; }

  _NODISCARD _CONSTEXPR17 reference operator*() const {
    _BidIt _Tmp = current;
    return *--_Tmp;
  }

  _NODISCARD _CONSTEXPR17 pointer operator->() const {
    _BidIt _Tmp = current;
    --_Tmp;
    return _Operator_arrow(_Tmp, is_pointer<_BidIt>());
  }

  _CONSTEXPR17 reverse_iterator& operator++() {
    --current;
    return *this;
  }

  _CONSTEXPR17 reverse_iterator operator++(int) {
    reverse_iterator _Tmp = *this;
    --current;
    return _Tmp;
  }

  _CONSTEXPR17 reverse_iterator& operator--() {
    ++current;
    return *this;
  }

  _CONSTEXPR17 reverse_iterator operator--(int) {
    reverse_iterator _Tmp = *this;
    ++current;
    return _Tmp;
  }

  // N.B. functions valid for random-access iterators only beyond this point

  _CONSTEXPR17 reverse_iterator& operator+=(const difference_type _Off) {
    current -= _Off;
    return *this;
  }

  _NODISCARD _CONSTEXPR17 reverse_iterator
  operator+(const difference_type _Off) const {
    return reverse_iterator(current - _Off);
  }

  _CONSTEXPR17 reverse_iterator& operator-=(const difference_type _Off) {
    current += _Off;
    return *this;
  }

  _NODISCARD _CONSTEXPR17 reverse_iterator
  operator-(const difference_type _Off) const {
    return reverse_iterator(current + _Off);
  }

  _NODISCARD _CONSTEXPR17 reference
  operator[](const difference_type _Off) const {
    return current[static_cast<difference_type>(-_Off - 1)];
  }

  using _Prevent_inheriting_unwrap = reverse_iterator;

  template <class _BidIt2 = _BidIt,
            enable_if_t<_Offset_verifiable_v<_BidIt2>, int> = 0>
  constexpr void _Verify_offset(const difference_type _Off) const {
    _STL_VERIFY(_Off != _Min_possible_v<difference_type>, "integer overflow");
    current._Verify_offset(-_Off);
  }

  template <class _BidIt2 = _BidIt,
            enable_if_t<_Unwrappable_v<_BidIt2>, int> = 0>
  _NODISCARD constexpr reverse_iterator<_Unwrapped_t<_BidIt2>> _Unwrapped()
      const {
    return static_cast<reverse_iterator<_Unwrapped_t<_BidIt2>>>(
        current._Unwrapped());
  }

  static constexpr bool _Unwrap_when_unverified =
      _Do_unwrap_when_unverified_v<_BidIt>;

  template <class _Src, enable_if_t<_Wrapped_seekable_v<_BidIt, _Src>, int> = 0>
  constexpr void _Seek_to(const reverse_iterator<_Src>& _It) {
    current._Seek_to(_It.base());
  }

 protected:
  _BidIt current;  // the wrapped iterator
};

template <class _BidIt, class _BidIt2,
          enable_if_t<_Range_verifiable_v<_BidIt, _BidIt2>, int> = 0>
constexpr void _Verify_range(const reverse_iterator<_BidIt>& _First,
                             const reverse_iterator<_BidIt2>& _Last) {
  // TRANSITION, VSO#612785
  _Verify_range(_Last.base(), _First.base());  // note reversed parameters
}

template <class _BidIt>
_NODISCARD _CONSTEXPR17 reverse_iterator<_BidIt> operator+(
    typename reverse_iterator<_BidIt>::difference_type _Off,
    const reverse_iterator<_BidIt>& _Right) {
  return _Right + _Off;
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 auto operator-(const reverse_iterator<_BidIt1>& _Left,
                                       const reverse_iterator<_BidIt2>& _Right)
    -> decltype(_Right.base() - _Left.base()) {
  return _Right.base() - _Left.base();
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator==(
    const reverse_iterator<_BidIt1>& _Left,
    const reverse_iterator<_BidIt2>& _Right) {
  return _Left.base() == _Right.base();
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator!=(
    const reverse_iterator<_BidIt1>& _Left,
    const reverse_iterator<_BidIt2>& _Right) {
  return !(_Left == _Right);
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator<(
    const reverse_iterator<_BidIt1>& _Left,
    const reverse_iterator<_BidIt2>& _Right) {
  return _Right.base() < _Left.base();
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator>(
    const reverse_iterator<_BidIt1>& _Left,
    const reverse_iterator<_BidIt2>& _Right) {
  return _Right < _Left;
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator<=(
    const reverse_iterator<_BidIt1>& _Left,
    const reverse_iterator<_BidIt2>& _Right) {
  return !(_Right < _Left);
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator>=(
    const reverse_iterator<_BidIt1>& _Left,
    const reverse_iterator<_BidIt2>& _Right) {
  return !(_Left < _Right);
}

// FUNCTION TEMPLATE make_reverse_iterator
template <class _BidIt>
_NODISCARD _CONSTEXPR17 reverse_iterator<_BidIt> make_reverse_iterator(
    _BidIt _Iter) {  // make reverse_iterator from iterator
  return reverse_iterator<_BidIt>(_Iter);
}

// FUNCTION TEMPLATES begin AND end
template <class _Container>
_NODISCARD _CONSTEXPR17 auto begin(_Container& _Cont)
    -> decltype(_Cont.begin()) {  // get beginning of sequence
  return _Cont.begin();
}

template <class _Container>
_NODISCARD _CONSTEXPR17 auto begin(const _Container& _Cont)
    -> decltype(_Cont.begin()) {  // get beginning of sequence
  return _Cont.begin();
}

template <class _Container>
_NODISCARD _CONSTEXPR17 auto end(_Container& _Cont)
    -> decltype(_Cont.end()) {  // get end of sequence
  return _Cont.end();
}

template <class _Container>
_NODISCARD _CONSTEXPR17 auto end(const _Container& _Cont)
    -> decltype(_Cont.end()) {  // get end of sequence
  return _Cont.end();
}

template <class _Ty, size_t _Size>
_NODISCARD constexpr _Ty* begin(
    _Ty (&_Array)[_Size]) noexcept {  // get beginning of array
  return _Array;
}

template <class _Ty, size_t _Size>
_NODISCARD constexpr _Ty* end(
    _Ty (&_Array)[_Size]) noexcept {  // get end of array
  return _Array + _Size;
}

// FUNCTION TEMPLATES cbegin AND cend
template <class _Container>
_NODISCARD constexpr auto cbegin(const _Container& _Cont)
    _NOEXCEPT_COND(noexcept(_STD begin(_Cont)))
        -> decltype(_STD begin(_Cont)) {  // get beginning of sequence
  return _STD begin(_Cont);
}

template <class _Container>
_NODISCARD constexpr auto cend(const _Container& _Cont)
    _NOEXCEPT_COND(noexcept(_STD end(_Cont)))
        -> decltype(_STD end(_Cont)) {  // get end of sequence
  return _STD end(_Cont);
}

// FUNCTION TEMPLATES rbegin AND rend
template <class _Container>
_NODISCARD _CONSTEXPR17 auto rbegin(_Container& _Cont)
    -> decltype(_Cont.rbegin()) {  // get beginning of reversed sequence
  return _Cont.rbegin();
}

template <class _Container>
_NODISCARD _CONSTEXPR17 auto rbegin(const _Container& _Cont)
    -> decltype(_Cont.rbegin()) {  // get beginning of reversed sequence
  return _Cont.rbegin();
}

template <class _Container>
_NODISCARD _CONSTEXPR17 auto rend(_Container& _Cont)
    -> decltype(_Cont.rend()) {  // get end of reversed sequence
  return _Cont.rend();
}

template <class _Container>
_NODISCARD _CONSTEXPR17 auto rend(const _Container& _Cont)
    -> decltype(_Cont.rend()) {  // get end of reversed sequence
  return _Cont.rend();
}

template <class _Ty,
          size_t _Size>
_NODISCARD _CONSTEXPR17 reverse_iterator<_Ty*> rbegin(
    _Ty (&_Array)[_Size]) {  // get beginning of reversed array
  return reverse_iterator<_Ty*>(_Array + _Size);
}

template <class _Ty,
          size_t _Size>
_NODISCARD _CONSTEXPR17 reverse_iterator<_Ty*> rend(
    _Ty (&_Array)[_Size]) {  // get end of reversed array
  return reverse_iterator<_Ty*>(_Array);
}

template <class _Elem>
_NODISCARD _CONSTEXPR17 reverse_iterator<const _Elem*> rbegin(
    initializer_list<_Elem> _Ilist) {  // get beginning of reversed sequence
  return reverse_iterator<const _Elem*>(_Ilist.end());
}

template <class _Elem>
_NODISCARD _CONSTEXPR17 reverse_iterator<const _Elem*> rend(
    initializer_list<_Elem> _Ilist) {  // get end of reversed sequence
  return reverse_iterator<const _Elem*>(_Ilist.begin());
}

// FUNCTION TEMPLATES crbegin AND crend
template <class _Container>
_NODISCARD _CONSTEXPR17 auto crbegin(const _Container& _Cont)
    -> decltype(_STD rbegin(_Cont)) {  // get beginning of reversed sequence
  return _STD rbegin(_Cont);
}

template <class _Container>
_NODISCARD _CONSTEXPR17 auto crend(const _Container& _Cont)
    -> decltype(_STD rend(_Cont)) {  // get end of reversed sequence
  return _STD rend(_Cont);
}

template <class _Container>
_NODISCARD constexpr auto size(const _Container& _Cont)
    -> decltype(_Cont.size()) {  // get size() for container
  return _Cont.size();
}

template <class _Ty, size_t _Size>
_NODISCARD constexpr size_t size(
    const _Ty (&)[_Size]) noexcept {  // get dimension for array
  return _Size;
}

template <class _Container>
_NODISCARD constexpr auto empty(const _Container& _Cont)
    -> decltype(_Cont.empty()) {  // get empty() for container
  return _Cont.empty();
}

template <class _Ty, size_t _Size>
_NODISCARD constexpr bool empty(
    const _Ty (&)[_Size]) noexcept {  // get dimension==0 for array (can't
                                      // happen)
  return false;
}

template <class _Elem>
_NODISCARD constexpr bool empty(
    initializer_list<_Elem>
        _Ilist) noexcept {  // get dimension==0 for initializer_list
  return _Ilist.size() == 0;
}

template <class _Container>
_NODISCARD constexpr auto data(_Container& _Cont)
    -> decltype(_Cont.data()) {  // get data() for container
  return _Cont.data();
}

template <class _Container>
_NODISCARD constexpr auto data(const _Container& _Cont)
    -> decltype(_Cont.data()) {  // get pointer to data of const container
  return _Cont.data();
}

template <class _Ty, size_t _Size>
_NODISCARD constexpr _Ty* data(
    _Ty (&_Array)[_Size]) noexcept {  // get pointer to data of array
  return _Array;
}

template <class _Elem>
_NODISCARD constexpr const _Elem* data(
    initializer_list<_Elem>
        _Ilist) noexcept {  // get pointer to data of initializer_list
  return _Ilist.begin();
}

// MACRO _STL_VERIFY_ARRAY_SIZE
#define _STL_VERIFY_ARRAY_SIZE(_Array, _Desired)                          \
  _STL_VERIFY(                                                            \
      (_STD size(_Array) >=                                               \
       static_cast<common_type_t<size_t, decltype(_Desired)>>(_Desired)), \
      "array too small")

struct _Container_proxy;
struct _Iterator_base12;

// CLASS TEMPLATE _Array_const_iterator
#if _ITERATOR_DEBUG_LEVEL != 0
struct _Iterator_base12_compatible {  // TRANSITION, for binary compatibility
  _Container_proxy* _Myproxy{};
  _Iterator_base12* _Mynextiter{};
};
#endif  // _ITERATOR_DEBUG_LEVEL != 0

template <class _Ty, size_t _Size>
class _Array_const_iterator
#if _ITERATOR_DEBUG_LEVEL != 0
    : private _Iterator_base12_compatible
#endif  // _ITERATOR_DEBUG_LEVEL != 0
{       // iterator for nonmutable array
 public:
  using iterator_category = random_access_iterator_tag;

  using value_type = _Ty;
  using difference_type = ptrdiff_t;
  using pointer = const _Ty*;
  using reference = const _Ty&;
  enum { _EEN_SIZE = _Size };  // helper for expression evaluator
#if _ITERATOR_DEBUG_LEVEL == 0
  _CONSTEXPR17 _Array_const_iterator()
      : _Ptr() {  // construct with null pointer
  }

  _CONSTEXPR17 explicit _Array_const_iterator(pointer _Parg, size_t _Off = 0)
      : _Ptr(_Parg + _Off) {}

  _NODISCARD _CONSTEXPR17 reference operator*() const { return *_Ptr; }

  _NODISCARD _CONSTEXPR17 pointer operator->() const { return _Ptr; }

  _CONSTEXPR17 _Array_const_iterator& operator++() {
    ++_Ptr;
    return *this;
  }

  _CONSTEXPR17 _Array_const_iterator operator++(int) {
    _Array_const_iterator _Tmp = *this;
    ++_Ptr;
    return _Tmp;
  }

  _CONSTEXPR17 _Array_const_iterator& operator--() {
    --_Ptr;
    return *this;
  }

  _CONSTEXPR17 _Array_const_iterator operator--(int) {
    _Array_const_iterator _Tmp = *this;
    --_Ptr;
    return _Tmp;
  }

  _CONSTEXPR17 _Array_const_iterator& operator+=(const ptrdiff_t _Off) {
    _Ptr += _Off;
    return *this;
  }

  _NODISCARD _CONSTEXPR17 _Array_const_iterator
  operator+(const ptrdiff_t _Off) const {
    _Array_const_iterator _Tmp = *this;
    return _Tmp += _Off;
  }

  _CONSTEXPR17 _Array_const_iterator& operator-=(const ptrdiff_t _Off) {
    _Ptr -= _Off;
    return *this;
  }

  _NODISCARD _CONSTEXPR17 _Array_const_iterator
  operator-(const ptrdiff_t _Off) const {
    _Array_const_iterator _Tmp = *this;
    return _Tmp -= _Off;
  }

  _NODISCARD _CONSTEXPR17 ptrdiff_t
  operator-(const _Array_const_iterator& _Right) const {
    return _Ptr - _Right._Ptr;
  }

  _NODISCARD _CONSTEXPR17 reference operator[](const ptrdiff_t _Off) const {
    return _Ptr[_Off];
  }

  _NODISCARD _CONSTEXPR17 bool operator==(
      const _Array_const_iterator& _Right) const {
    return _Ptr == _Right._Ptr;
  }

  _NODISCARD _CONSTEXPR17 bool operator!=(
      const _Array_const_iterator& _Right) const {
    return !(*this == _Right);
  }

  _NODISCARD _CONSTEXPR17 bool operator<(
      const _Array_const_iterator& _Right) const {
    return _Ptr < _Right._Ptr;
  }

  _NODISCARD _CONSTEXPR17 bool operator>(
      const _Array_const_iterator& _Right) const {
    return _Right < *this;
  }

  _NODISCARD _CONSTEXPR17 bool operator<=(
      const _Array_const_iterator& _Right) const {
    return !(_Right < *this);
  }

  _NODISCARD _CONSTEXPR17 bool operator>=(
      const _Array_const_iterator& _Right) const {
    return !(*this < _Right);
  }

  using _Prevent_inheriting_unwrap = _Array_const_iterator;

  _NODISCARD constexpr pointer _Unwrapped() const { return _Ptr; }

  static constexpr bool _Unwrap_when_unverified = true;

  constexpr void _Seek_to(pointer _It) { _Ptr = _It; }

 private:
  pointer _Ptr;  // beginning of array

#else  // ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0
       // vvv
  _CONSTEXPR17 _Array_const_iterator()
      : _Ptr(), _Idx(0) {  // construct with null pointer
  }

  _CONSTEXPR17 explicit _Array_const_iterator(pointer _Parg, size_t _Off = 0)
      : _Ptr(_Parg), _Idx(_Off) {}

  _NODISCARD _CONSTEXPR17 reference operator*() const { return *operator->(); }

  _NODISCARD _CONSTEXPR17 pointer operator->() const {
    _STL_VERIFY(_Ptr, "cannot dereference value-initialized array iterator");
    _STL_VERIFY(_Idx < _Size, "cannot dereference out of range array iterator");
    return _Ptr + _Idx;
  }

  _CONSTEXPR17 _Array_const_iterator& operator++() {
    _STL_VERIFY(_Ptr, "cannot increment value-initialized array iterator");
    _STL_VERIFY(_Idx < _Size, "cannot increment array iterator past end");
    ++_Idx;
    return *this;
  }

  _CONSTEXPR17 _Array_const_iterator operator++(int) {
    _Array_const_iterator _Tmp = *this;
    ++*this;
    return _Tmp;
  }

  _CONSTEXPR17 _Array_const_iterator& operator--() {
    _STL_VERIFY(_Ptr, "cannot decrement value-initialized array iterator");
    _STL_VERIFY(_Idx != 0, "cannot decrement array iterator before begin");
    --_Idx;
    return *this;
  }

  _CONSTEXPR17 _Array_const_iterator operator--(int) {
    _Array_const_iterator _Tmp = *this;
    --*this;
    return _Tmp;
  }

  constexpr void _Verify_offset(const ptrdiff_t _Off) const noexcept {
    if (_Off != 0) {
      _STL_VERIFY(_Ptr, "cannot seek value-initialized array iterator");
    }

    if (_Off < 0) {
#pragma warning(suppress : 4146)  // unary minus operator applied to unsigned
                                  // type, result still unsigned
      _STL_VERIFY(_Idx >= -static_cast<size_t>(_Off),
                  "cannot seek array iterator before begin");
    }

    if (_Off > 0) {
      _STL_VERIFY(_Size - _Idx >= static_cast<size_t>(_Off),
                  "cannot seek array iterator after end");
    }
  }

  _CONSTEXPR17 _Array_const_iterator& operator+=(const ptrdiff_t _Off) {
    _Verify_offset(_Off);
    _Idx += _Off;
    return *this;
  }

  _NODISCARD _CONSTEXPR17 _Array_const_iterator
  operator+(const ptrdiff_t _Off) const {
    _Array_const_iterator _Tmp = *this;
    return _Tmp += _Off;
  }

  _CONSTEXPR17 _Array_const_iterator& operator-=(const ptrdiff_t _Off) {
    return *this += -_Off;
  }

  _NODISCARD _CONSTEXPR17 _Array_const_iterator
  operator-(const ptrdiff_t _Off) const {
    _Array_const_iterator _Tmp = *this;
    return _Tmp -= _Off;
  }

  _NODISCARD _CONSTEXPR17 ptrdiff_t
  operator-(const _Array_const_iterator& _Right) const {
    _Compat(_Right);
    return static_cast<ptrdiff_t>(_Idx - _Right._Idx);
  }

  _NODISCARD _CONSTEXPR17 reference operator[](const ptrdiff_t _Off) const {
    return *(*this + _Off);
  }

  _NODISCARD _CONSTEXPR17 bool operator==(
      const _Array_const_iterator& _Right) const {
    _Compat(_Right);
    return _Idx == _Right._Idx;
  }

  _NODISCARD _CONSTEXPR17 bool operator!=(
      const _Array_const_iterator& _Right) const {
    return !(*this == _Right);
  }

  _NODISCARD _CONSTEXPR17 bool operator<(
      const _Array_const_iterator& _Right) const {
    _Compat(_Right);
    return _Idx < _Right._Idx;
  }

  _NODISCARD _CONSTEXPR17 bool operator>(
      const _Array_const_iterator& _Right) const {
    return _Right < *this;
  }

  _NODISCARD _CONSTEXPR17 bool operator<=(
      const _Array_const_iterator& _Right) const {
    return !(_Right < *this);
  }

  _NODISCARD _CONSTEXPR17 bool operator>=(
      const _Array_const_iterator& _Right) const {
    return !(*this < _Right);
  }

  _CONSTEXPR17 void _Compat(const _Array_const_iterator& _Right)
      const {  // test for compatible iterator pair
    _STL_VERIFY(_Ptr == _Right._Ptr, "array iterators incompatible");
  }

  using _Prevent_inheriting_unwrap = _Array_const_iterator;

  _NODISCARD constexpr pointer _Unwrapped() const { return _Ptr + _Idx; }

  constexpr void _Verify_with(const _Array_const_iterator& _Last) const
      noexcept {
    // note _Compat check inside operator<=
    _STL_VERIFY(*this <= _Last, "array iterator range transposed");
  }

  constexpr void _Seek_to(pointer _It) {
    _Idx = static_cast<size_t>(_It - _Ptr);
  }

 private:
  pointer _Ptr;  // beginning of array
  size_t _Idx;   // offset into array
#endif                            // _ITERATOR_DEBUG_LEVEL == 0
};

#if _ITERATOR_DEBUG_LEVEL != 0
template <class _Ty, size_t _Size>
constexpr void _Verify_range(
    const _Array_const_iterator<_Ty, _Size>& _First,
    const _Array_const_iterator<_Ty, _Size>& _Last) noexcept {
  // TRANSITION, VSO#612785
  _First._Verify_with(_Last);
}
#endif  // _ITERATOR_DEBUG_LEVEL != 0

template <class _Ty, size_t _Size>
_NODISCARD _CONSTEXPR17 _Array_const_iterator<_Ty, _Size> operator+(
    ptrdiff_t _Off, _Array_const_iterator<_Ty, _Size> _Next) {
  return _Next += _Off;
}

// CLASS TEMPLATE _Array_iterator
template <class _Ty, size_t _Size>
class _Array_iterator
    : public _Array_const_iterator<_Ty, _Size> {  // iterator for mutable array
 public:
  using _Mybase = _Array_const_iterator<_Ty, _Size>;
  using iterator_category = random_access_iterator_tag;

  using value_type = _Ty;
  using difference_type = ptrdiff_t;
  using pointer = _Ty*;
  using reference = _Ty&;

  _CONSTEXPR17 _Array_iterator() {  // construct with null pointer
  }

  _CONSTEXPR17 explicit _Array_iterator(pointer _Parg, size_t _Off = 0)
      : _Mybase(_Parg, _Off) {}

  enum { _EEN_SIZE = _Size };  // helper for expression evaluator

  _NODISCARD _CONSTEXPR17 reference operator*() const {
    return const_cast<reference>(_Mybase::operator*());
  }

  _NODISCARD _CONSTEXPR17 pointer operator->() const {
    return const_cast<pointer>(_Mybase::operator->());
  }

  _CONSTEXPR17 _Array_iterator& operator++() {
    _Mybase::operator++();
    return *this;
  }

  _CONSTEXPR17 _Array_iterator operator++(int) {
    _Array_iterator _Tmp = *this;
    _Mybase::operator++();
    return _Tmp;
  }

  _CONSTEXPR17 _Array_iterator& operator--() {
    _Mybase::operator--();
    return *this;
  }

  _CONSTEXPR17 _Array_iterator operator--(int) {
    _Array_iterator _Tmp = *this;
    _Mybase::operator--();
    return _Tmp;
  }

  _CONSTEXPR17 _Array_iterator& operator+=(const ptrdiff_t _Off) {
    _Mybase::operator+=(_Off);
    return *this;
  }

  _NODISCARD _CONSTEXPR17 _Array_iterator
  operator+(const ptrdiff_t _Off) const {
    _Array_iterator _Tmp = *this;
    return _Tmp += _Off;
  }

  _CONSTEXPR17 _Array_iterator& operator-=(const ptrdiff_t _Off) {
    _Mybase::operator-=(_Off);
    return *this;
  }

  using _Mybase::operator-;

  _NODISCARD _CONSTEXPR17 _Array_iterator
  operator-(const ptrdiff_t _Off) const {
    _Array_iterator _Tmp = *this;
    return _Tmp -= _Off;
  }

  _NODISCARD _CONSTEXPR17 reference operator[](const ptrdiff_t _Off) const {
    return const_cast<reference>(_Mybase::operator[](_Off));
  }

  using _Prevent_inheriting_unwrap = _Array_iterator;

  _NODISCARD constexpr pointer _Unwrapped() const {
    return const_cast<pointer>(_Mybase::_Unwrapped());
  }
};

template <class _Ty, size_t _Size>
_NODISCARD _CONSTEXPR17 _Array_iterator<_Ty, _Size> operator+(
    ptrdiff_t _Off, _Array_iterator<_Ty, _Size> _Next) {
  return _Next += _Off;
}

// STRUCT _Default_sentinel
struct _Default_sentinel {};  // empty struct to serve as the end of a range

// CLASS TEMPLATE move_iterator
template <class _Iter>
class move_iterator {  // wrap iterator to move rvalues
 public:
  using iterator_category = typename iterator_traits<_Iter>::iterator_category;
  using value_type = typename iterator_traits<_Iter>::value_type;
  using difference_type = typename iterator_traits<_Iter>::difference_type;
  using pointer = _Iter;
  using _Ref0 = typename iterator_traits<_Iter>::reference;
  using reference =
      conditional_t<is_reference_v<_Ref0>, remove_reference_t<_Ref0>&&, _Ref0>;
  using iterator_type = _Iter;

  _CONSTEXPR17 move_iterator() : current() {}

  _CONSTEXPR17 explicit move_iterator(iterator_type _Right)
      : current(_Right) {  // construct wrapped iterator from _Right
  }

  template <class _Iter2>
  _CONSTEXPR17 move_iterator(const move_iterator<_Iter2>& _Right)
      : current(_Right.base()) {}

  template <class _Iter2>
  _CONSTEXPR17 move_iterator& operator=(const move_iterator<_Iter2>& _Right) {
    current = _Right.base();
    return *this;
  }

  _NODISCARD _CONSTEXPR17 _Iter base() const { return current; }

  _NODISCARD _CONSTEXPR17 reference operator*() const {
    return static_cast<reference>(*current);
  }

  _NODISCARD _CONSTEXPR17 pointer operator->() const { return current; }

  _CONSTEXPR17 move_iterator& operator++() {
    ++current;
    return *this;
  }

  _CONSTEXPR17 move_iterator operator++(int) {
    move_iterator _Tmp = *this;
    ++current;
    return _Tmp;
  }

  _CONSTEXPR17 move_iterator& operator--() {
    --current;
    return *this;
  }

  _CONSTEXPR17 move_iterator operator--(int) {
    move_iterator _Tmp = *this;
    --current;
    return _Tmp;
  }

  template <class _Iter2 = _Iter>
  _NODISCARD auto operator==(_Default_sentinel _Sentinel) const noexcept
      -> decltype(_STD declval<const _Iter2&>() == _Sentinel) {
    return current == _Sentinel;
  }

  template <class _Iter2 = _Iter>
  _NODISCARD auto operator!=(_Default_sentinel _Sentinel) const noexcept
      -> decltype(_STD declval<const _Iter2&>() != _Sentinel) {
    return current != _Sentinel;
  }

  // N.B. functions valid for random-access iterators only beyond this point

  _CONSTEXPR17 move_iterator& operator+=(const difference_type _Off) {
    current += _Off;
    return *this;
  }

  _NODISCARD _CONSTEXPR17 move_iterator
  operator+(const difference_type _Off) const {
    return move_iterator(current + _Off);
  }

  _CONSTEXPR17 move_iterator& operator-=(const difference_type _Off) {
    current -= _Off;
    return *this;
  }

  _NODISCARD _CONSTEXPR17 move_iterator
  operator-(const difference_type _Off) const {
    return move_iterator(current - _Off);
  }

  _NODISCARD _CONSTEXPR17 reference
  operator[](const difference_type _Off) const {
    return _STD move(current[_Off]);
  }

  template <class _Iter2,
            enable_if_t<_Range_verifiable_v<_Iter, _Iter2>, int> = 0>
  friend constexpr void _Verify_range(const move_iterator& _First,
                                      const move_iterator<_Iter2>& _Last) {
    _Verify_range(_First.base(), _Last.base());
  }

  using _Prevent_inheriting_unwrap = move_iterator;

  template <class _Iter2 = iterator_type,
            enable_if_t<_Offset_verifiable_v<_Iter2>, int> = 0>
  constexpr void _Verify_offset(const difference_type _Off) const {
    current._Verify_offset(_Off);
  }

  template <class _Iter2 = iterator_type,
            enable_if_t<_Unwrappable_v<_Iter2>, int> = 0>
  _NODISCARD constexpr move_iterator<_Unwrapped_t<_Iter2>> _Unwrapped() const {
    return static_cast<move_iterator<_Unwrapped_t<_Iter2>>>(
        current._Unwrapped());
  }

  static constexpr bool _Unwrap_when_unverified =
      _Do_unwrap_when_unverified_v<iterator_type>;

  template <class _Src,
            enable_if_t<_Wrapped_seekable_v<iterator_type, _Src>, int> = 0>
  constexpr void _Seek_to(const move_iterator<_Src>& _It) {
    current._Seek_to(_It.base());
  }

 protected:
  iterator_type current;  // the wrapped iterator
};

template <class _Iter>
_NODISCARD _CONSTEXPR17 move_iterator<_Iter> operator+(
    typename move_iterator<_Iter>::difference_type _Off,
    const move_iterator<_Iter>& _Right) {  // return move_iterator + integer
  return _Right + _Off;
}

template <class _Iter1, class _Iter2>
_NODISCARD _CONSTEXPR17 auto operator-(const move_iterator<_Iter1>& _Left,
                                       const move_iterator<_Iter2>& _Right)
    -> decltype(_Left.base() - _Right.base()) {
  return _Left.base() - _Right.base();
}

template <class _Iter1, class _Iter2>
_NODISCARD _CONSTEXPR17 bool operator==(const move_iterator<_Iter1>& _Left,
                                        const move_iterator<_Iter2>& _Right) {
  return _Left.base() == _Right.base();
}

template <class _Iter1, class _Iter2>
_NODISCARD _CONSTEXPR17 bool operator!=(const move_iterator<_Iter1>& _Left,
                                        const move_iterator<_Iter2>& _Right) {
  return !(_Left == _Right);
}

template <class _Iter1, class _Iter2>
_NODISCARD _CONSTEXPR17 bool operator<(const move_iterator<_Iter1>& _Left,
                                       const move_iterator<_Iter2>& _Right) {
  return _Left.base() < _Right.base();
}

template <class _Iter1, class _Iter2>
_NODISCARD _CONSTEXPR17 bool operator>(const move_iterator<_Iter1>& _Left,
                                       const move_iterator<_Iter2>& _Right) {
  return _Right < _Left;
}

template <class _Iter1, class _Iter2>
_NODISCARD _CONSTEXPR17 bool operator<=(const move_iterator<_Iter1>& _Left,
                                        const move_iterator<_Iter2>& _Right) {
  return !(_Right < _Left);
}

template <class _Iter1, class _Iter2>
_NODISCARD _CONSTEXPR17 bool operator>=(const move_iterator<_Iter1>& _Left,
                                        const move_iterator<_Iter2>& _Right) {
  return !(_Left < _Right);
}

// FUNCTION TEMPLATE make_move_iterator
template <class _Iter>
_NODISCARD _CONSTEXPR17 move_iterator<_Iter> make_move_iterator(
    _Iter _It) {  // make move_iterator from iterator
  return move_iterator<_Iter>(_It);
}

// FUNCTION TEMPLATE copy
template <class _InIt, class _OutIt>
inline _OutIt _Copy_memmove(
    _InIt _First, _InIt _Last,
    _OutIt _Dest) {  // implement copy-like function as memmove
  const char* const _First_ch =
      const_cast<const char*>(reinterpret_cast<const volatile char*>(_First));
  const char* const _Last_ch =
      const_cast<const char*>(reinterpret_cast<const volatile char*>(_Last));
  char* const _Dest_ch =
      const_cast<char*>(reinterpret_cast<volatile char*>(_Dest));
  const auto _Count = static_cast<size_t>(_Last_ch - _First_ch);
  _CSTD memmove(_Dest_ch, _First_ch, _Count);
  return reinterpret_cast<_OutIt>(_Dest_ch + _Count);
}

template <class _InIt, class _OutIt>
inline _OutIt _Copy_memmove(move_iterator<_InIt> _First,
                            move_iterator<_InIt> _Last, _OutIt _Dest) {
  return _Copy_memmove(_First.base(), _Last.base(), _Dest);
}

#if _HAS_IF_CONSTEXPR
template <class _InIt, class _OutIt>
inline _OutIt _Copy_unchecked(
    _InIt _First, _InIt _Last,
    _OutIt _Dest) {  // copy [_First, _Last) to [_Dest, ...)
                     // note: _Copy_unchecked is called directly from elsewhere
                     // in the STL
  if constexpr (_Ptr_copy_cat<_InIt, _OutIt>::_Trivially_copyable) {
    return _Copy_memmove(_First, _Last, _Dest);
  } else {
    for (; _First != _Last; ++_Dest, (void)++_First) {
      *_Dest = *_First;
    }

    return _Dest;
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _InIt, class _OutIt>
inline _OutIt _Copy_unchecked1(
    _InIt _First, _InIt _Last, _OutIt _Dest,
    false_type) {  // copy [_First, _Last) to [_Dest, ...), arbitrary iterators
  for (; _First != _Last; ++_Dest, (void)++_First) {
    *_Dest = *_First;
  }

  return _Dest;
}

template <class _InIt, class _OutIt>
inline _OutIt _Copy_unchecked1(
    _InIt _First, _InIt _Last, _OutIt _Dest,
    true_type) {  // copy [_First, _Last) to [_Dest, ...), pointers to trivially
                  // copyable
  return _Copy_memmove(_First, _Last, _Dest);
}

template <class _InIt, class _OutIt>
inline _OutIt _Copy_unchecked(
    _InIt _First, _InIt _Last,
    _OutIt _Dest) {  // copy [_First, _Last) to [_Dest, ...)
                     // note: _Copy_unchecked is called directly from elsewhere
                     // in the STL
  return _Copy_unchecked1(
      _First, _Last, _Dest,
      bool_constant<_Ptr_copy_cat<_InIt, _OutIt>::_Trivially_copyable>{});
}
#endif  // _HAS_IF_CONSTEXPR

template <class _InIt, class _OutIt>
inline _OutIt copy(_InIt _First, _InIt _Last,
                   _OutIt _Dest) {  // copy [_First, _Last) to [_Dest, ...)
  _Adl_verify_range(_First, _Last);
  const auto _UFirst = _Get_unwrapped(_First);
  const auto _ULast = _Get_unwrapped(_Last);
  const auto _UDest =
      _Get_unwrapped_n(_Dest, _Idl_distance<_InIt>(_UFirst, _ULast));
  _Seek_wrapped(_Dest, _Copy_unchecked(_UFirst, _ULast, _UDest));
  return _Dest;
}

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _InIt, class _DestTy, size_t _DestSize>
inline _DestTy* copy(
    _InIt _First, _InIt _Last,
    _DestTy (&_Dest)[_DestSize]) {  // copy [_First, _Last) to [_Dest, ...)
  return _STD copy(_First, _Last, _Array_iterator<_DestTy, _DestSize>(_Dest))
      ._Unwrapped();
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS

#if _HAS_CXX17
template <class _ExPo, class _FwdIt1, class _FwdIt2,
          _Enable_if_execution_policy_t<_ExPo> = 0>
inline _FwdIt2 copy(_ExPo&&, _FwdIt1 _First, _FwdIt1 _Last,
                    _FwdIt2 _Dest) noexcept {
  // copy [_First, _Last) to [_Dest, ...)
  // not parallelized as benchmarks show it isn't worth it
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt2);
  return _STD copy(_First, _Last, _Dest);
}

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _ExPo, class _FwdIt1, class _DestTy, size_t _DestSize,
          _Enable_if_execution_policy_t<_ExPo> = 0>
inline _DestTy* copy(_ExPo&&, _FwdIt1 _First, _FwdIt1 _Last,
                     _DestTy (&_Dest)[_DestSize]) noexcept {
  // copy [_First, _Last) to [_Dest, ...)
  // not parallelized as benchmarks show it isn't worth it
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
  return _STD copy(_First, _Last, _Dest);
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE copy_n
// TRANSITION: _Copy_n_unchecked, _Copy_n_unchecked1, _Copy_n_unchecked2, and
// _Copy_n_unchecked3 are ABI zombie names
#if _HAS_IF_CONSTEXPR
template <class _InIt, class _Diff, class _OutIt>
inline _OutIt copy_n(
    _InIt _First, _Diff _Count_raw,
    _OutIt _Dest) {  // copy [_First, _First + _Count) to [_Dest, ...)
  _Algorithm_int_t<_Diff> _Count = _Count_raw;
  if (0 < _Count) {
    auto _UFirst = _Get_unwrapped_n(_First, _Count);
    auto _UDest = _Get_unwrapped_n(_Dest, _Count);
    if constexpr (_Ptr_copy_cat<decltype(_UFirst),
                                decltype(_UDest)>::_Trivially_copyable) {
      _UDest = _Copy_memmove(_UFirst, _UFirst + _Count, _UDest);
    } else {
      for (;;) {
        *_UDest = *_UFirst;
        ++_UDest;
        --_Count;
        if (_Count == 0) {  // note that we avoid an extra ++_First here to
                            // allow istream_iterator to work, see LWG#2471
          break;
        }

        ++_UFirst;
      }
    }

    _Seek_wrapped(_Dest, _UDest);
  }

  return _Dest;
}

#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _InIt, class _Diff, class _OutIt>
inline _OutIt _Copy_n_unchecked4(_InIt _First, _Diff _Count, _OutIt _Dest,
                                 false_type) {
  // copy [_First, _First + _Count) to [_Dest, ...), no special optimization
  // pre: 0 < _Count
  for (;;) {
    *_Dest = *_First;
    ++_Dest;
    --_Count;
    if (_Count == 0) {  // note that we avoid an extra ++_First here to allow
                        // istream_iterator to work, see LWG#2471
      return _Dest;
    }

    ++_First;
  }
}

template <class _InIt, class _Diff, class _OutIt>
inline _OutIt _Copy_n_unchecked4(_InIt _First, _Diff _Count, _OutIt _Dest,
                                 true_type) {
  // copy [_First, _First + _Count) to [_Dest, ...), memmove optimization
  // pre: 0 < _Count
  return _Copy_memmove(_First, _First + _Count, _Dest);
}

template <class _InIt, class _Diff, class _OutIt>
inline _OutIt copy_n(
    _InIt _First, _Diff _Count_raw,
    _OutIt _Dest) {  // copy [_First, _First + _Count) to [_Dest, ...)
  const _Algorithm_int_t<_Diff> _Count = _Count_raw;
  if (0 < _Count) {
    auto _UFirst = _Get_unwrapped_n(_First, _Count);
    auto _UDest = _Get_unwrapped_n(_Dest, _Count);
    _Seek_wrapped(
        _Dest,
        _Copy_n_unchecked4(
            _UFirst, _Count, _UDest,
            bool_constant<_Ptr_copy_cat<
                decltype(_UFirst), decltype(_UDest)>::_Trivially_copyable>{}));
  }

  return _Dest;
}
#endif  // _HAS_IF_CONSTEXPR

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _SourceTy, size_t _SourceSize, class _Diff, class _OutIt>
inline _OutIt copy_n(_SourceTy (&_First)[_SourceSize], _Diff _Count_raw,
                     _OutIt _Dest) {
  // copy [_First, _First + _Count) to [_Dest, ...), array source
  const _Algorithm_int_t<_Diff> _Count = _Count_raw;
  if (0 < _Count) {
    _STL_VERIFY_ARRAY_SIZE(_First, _Count);
    _Seek_wrapped(_Dest, _STD copy_n(static_cast<_SourceTy*>(_First), _Count,
                                     _Get_unwrapped_n(_Dest, _Count)));
  }

  return _Dest;
}

template <class _InIt, class _Diff, class _DestTy, size_t _DestSize>
inline _DestTy* copy_n(_InIt _First, _Diff _Count_raw,
                       _DestTy (&_Dest)[_DestSize]) {
  // copy [_First, _First + _Count) to [_Dest, ...), array dest
  const _Algorithm_int_t<_Diff> _Count = _Count_raw;
  if (0 < _Count) {
    _STL_VERIFY_ARRAY_SIZE(_Dest, _Count);
    return _STD copy_n(_First, _Count, static_cast<_DestTy*>(_Dest));
  }

  return _Dest;
}

template <class _SourceTy, size_t _SourceSize, class _Diff, class _DestTy,
          size_t _DestSize>
inline _DestTy* copy_n(_SourceTy (&_First)[_SourceSize], _Diff _Count_raw,
                       _DestTy (&_Dest)[_DestSize]) {
  // copy [_First, _First + _Count) to [_Dest, ...), array source/dest
  const _Algorithm_int_t<_Diff> _Count = _Count_raw;
  if (0 < _Count) {
    _STL_VERIFY_ARRAY_SIZE(_First, _Count);
    _STL_VERIFY_ARRAY_SIZE(_Dest, _Count);
    return _STD copy_n(static_cast<_SourceTy*>(_First), _Count,
                       static_cast<_DestTy*>(_Dest));
  }

  return _Dest;
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS

#if _HAS_CXX17
template <class _ExPo, class _FwdIt1, class _Diff, class _FwdIt2,
          _Enable_if_execution_policy_t<_ExPo> = 0>
inline _FwdIt2 copy_n(_ExPo&&, _FwdIt1 _First, _Diff _Count_raw,
                      _FwdIt2 _Dest) noexcept {
  // copy [_First, _First + _Count) to [_Dest, ...)
  // not parallelized as benchmarks show it isn't worth it
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt2);
  return _STD copy_n(_First, _Count_raw, _Dest);
}

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _ExPo, class _SourceTy, size_t _SourceSize, class _Diff,
          class _FwdIt2, _Enable_if_execution_policy_t<_ExPo> = 0>
inline _FwdIt2 copy_n(_ExPo&&, _SourceTy (&_First)[_SourceSize],
                      _Diff _Count_raw, _FwdIt2 _Dest) noexcept {
  // copy [_First, _First + _Count) to [_Dest, ...), array source
  // not parallelized as benchmarks show it isn't worth it
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt2);
  return _STD copy_n(_First, _Count_raw, _Dest);
}

template <class _ExPo, class _FwdIt1, class _Diff, class _DestTy,
          size_t _DestSize, _Enable_if_execution_policy_t<_ExPo> = 0>
inline _DestTy* copy_n(_ExPo&&, _FwdIt1 _First, _Diff _Count_raw,
                       _DestTy (&_Dest)[_DestSize]) noexcept {
  // copy [_First, _First + _Count) to [_Dest, ...), array dest
  // not parallelized as benchmarks show it isn't worth it
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
  return _STD copy_n(_First, _Count_raw, _Dest);
}

template <class _ExPo, class _SourceTy, size_t _SourceSize, class _Diff,
          class _DestTy, size_t _DestSize,
          _Enable_if_execution_policy_t<_ExPo> = 0>
inline _DestTy* copy_n(_ExPo&&, _SourceTy (&_First)[_SourceSize],
                       _Diff _Count_raw, _DestTy (&_Dest)[_DestSize]) noexcept {
  // copy [_First, _First + _Count) to [_Dest, ...), array source/dest
  // not parallelized as benchmarks show it isn't worth it
  return _STD copy_n(_First, _Count_raw, _Dest);
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE copy_backward
template <class _BidIt1, class _BidIt2>
inline _BidIt2 _Copy_backward_memmove(_BidIt1 _First, _BidIt1 _Last,
                                      _BidIt2 _Dest) {
  // implement copy_backward-like function as memmove
  const char* const _First_ch =
      const_cast<const char*>(reinterpret_cast<const volatile char*>(_First));
  const char* const _Last_ch =
      const_cast<const char*>(reinterpret_cast<const volatile char*>(_Last));
  char* const _Dest_ch =
      const_cast<char*>(reinterpret_cast<volatile char*>(_Dest));
  const auto _Count = static_cast<size_t>(_Last_ch - _First_ch);
  return static_cast<_BidIt2>(
      _CSTD memmove(_Dest_ch - _Count, _First_ch, _Count));
}

template <class _BidIt1, class _BidIt2>
inline _BidIt2 _Copy_backward_memmove(move_iterator<_BidIt1> _First,
                                      move_iterator<_BidIt1> _Last,
                                      _BidIt2 _Dest) {
  return _Copy_backward_memmove(_First.base(), _Last.base(), _Dest);
}

#if _HAS_IF_CONSTEXPR
template <class _BidIt1, class _BidIt2>
inline _BidIt2 copy_backward(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest) {
  // copy [_First, _Last) backwards to [..., _Dest)
  _Adl_verify_range(_First, _Last);
  const auto _UFirst = _Get_unwrapped(_First);
  auto _ULast = _Get_unwrapped(_Last);
  auto _UDest =
      _Get_unwrapped_n(_Dest, -_Idl_distance<_BidIt1>(_UFirst, _ULast));
  if constexpr (_Ptr_copy_cat<decltype(_ULast),
                              decltype(_UDest)>::_Trivially_copyable) {
    _UDest = _Copy_backward_memmove(_UFirst, _ULast, _UDest);
  } else {
    while (_UFirst != _ULast) {
      *--_UDest = *--_ULast;
    }
  }

  _Seek_wrapped(_Dest, _UDest);
  return _Dest;
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _BidIt1, class _BidIt2>
inline _BidIt2 _Copy_backward_unchecked(_BidIt1 _First, _BidIt1 _Last,
                                        _BidIt2 _Dest, false_type) {
  // copy [_First, _Last) backwards to [..., _Dest), no special optimization
  while (_First != _Last) {
    *--_Dest = *--_Last;
  }

  return _Dest;
}

template <class _BidIt1, class _BidIt2>
inline _BidIt2 _Copy_backward_unchecked(_BidIt1 _First, _BidIt1 _Last,
                                        _BidIt2 _Dest, true_type) {
  // copy [_First, _Last) backwards to [..., _Dest), memmove optimization
  return _Copy_backward_memmove(_First, _Last, _Dest);
}

template <class _BidIt1, class _BidIt2>
inline _BidIt2 copy_backward(
    _BidIt1 _First, _BidIt1 _Last,
    _BidIt2 _Dest) {  // copy [_First, _Last) backwards to [..., _Dest)
  _Adl_verify_range(_First, _Last);
  auto _UFirst = _Get_unwrapped(_First);
  const auto _ULast = _Get_unwrapped(_Last);
  auto _UDest =
      _Get_unwrapped_n(_Dest, -_Idl_distance<_BidIt1>(_UFirst, _ULast));
  _Seek_wrapped(
      _Dest,
      _Copy_backward_unchecked(
          _UFirst, _ULast, _UDest,
          bool_constant<_Ptr_copy_cat<
              decltype(_UFirst), decltype(_UDest)>::_Trivially_copyable>{}));
  return _Dest;
}
#endif  // _HAS_IF_CONSTEXPR

#if _HAS_CXX17
template <class _ExPo, class _BidIt1, class _BidIt2,
          _Enable_if_execution_policy_t<_ExPo> = 0>
inline _BidIt2 copy_backward(_ExPo&&, _BidIt1 _First, _BidIt1 _Last,
                             _BidIt2 _Dest) noexcept {
  // copy [_First, _Last) backwards to [..., _Dest)
  // not parallelized as benchmarks show it isn't worth it
  return _STD copy_backward(_First, _Last, _Dest);
}
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE move
#if _HAS_IF_CONSTEXPR
template <class _InIt, class _OutIt>
inline _OutIt _Move_unchecked(_InIt _First, _InIt _Last, _OutIt _Dest) {
  // move [_First, _Last) to [_Dest, ...)
  // note: _Move_unchecked is called directly from elsewhere in the STL
  if constexpr (_Ptr_move_cat<_InIt, _OutIt>::_Trivially_copyable) {
    return _Copy_memmove(_First, _Last, _Dest);
  } else {
    for (; _First != _Last; ++_Dest, (void)++_First) {
      *_Dest = _STD move(*_First);
    }

    return _Dest;
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _InIt, class _OutIt>
inline _OutIt _Move_unchecked1(_InIt _First, _InIt _Last, _OutIt _Dest,
                               false_type) {
  // move [_First, _Last) to [_Dest, ...), no special optimization
  for (; _First != _Last; ++_Dest, (void)++_First) {
    *_Dest = _STD move(*_First);
  }

  return _Dest;
}

template <class _InIt, class _OutIt>
inline _OutIt _Move_unchecked1(_InIt _First, _InIt _Last, _OutIt _Dest,
                               true_type) {
  // move [_First, _Last) to [_Dest, ...), memmove optimization
  return _Copy_memmove(_First, _Last, _Dest);
}

template <class _InIt, class _OutIt>
inline _OutIt _Move_unchecked(_InIt _First, _InIt _Last, _OutIt _Dest) {
  // move [_First, _Last) to [_Dest, ...), choose optimization
  // note: _Move_unchecked is called directly from elsewhere in the STL
  return _Move_unchecked1(
      _First, _Last, _Dest,
      bool_constant<_Ptr_move_cat<_InIt, _OutIt>::_Trivially_copyable>{});
}
#endif  // _HAS_IF_CONSTEXPR

template <class _InIt, class _OutIt>
inline _OutIt move(_InIt _First, _InIt _Last, _OutIt _Dest) {
  // move [_First, _Last) to [_Dest, ...)
  _Adl_verify_range(_First, _Last);
  const auto _UFirst = _Get_unwrapped(_First);
  const auto _ULast = _Get_unwrapped(_Last);
  const auto _UDest =
      _Get_unwrapped_n(_Dest, _Idl_distance<_InIt>(_UFirst, _ULast));
  _Seek_wrapped(_Dest, _Move_unchecked(_UFirst, _ULast, _UDest));
  return _Dest;
}

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _InIt, class _DestTy, size_t _DestSize>
inline _DestTy* move(
    _InIt _First, _InIt _Last,
    _DestTy (&_Dest)[_DestSize]) {  // move [_First, _Last) to [_Dest, ...)
  return _STD move(_First, _Last, _Array_iterator<_DestTy, _DestSize>(_Dest))
      ._Unwrapped();
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS

#if _HAS_CXX17
template <class _ExPo, class _FwdIt1, class _FwdIt2,
          _Enable_if_execution_policy_t<_ExPo> = 0>
inline _FwdIt2 move(_ExPo&&, _FwdIt1 _First, _FwdIt1 _Last,
                    _FwdIt2 _Dest) noexcept {
  // move [_First, _Last) to [_Dest, ...)
  // not parallelized as benchmarks show it isn't worth it
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt2);
  return _STD move(_First, _Last, _Dest);
}

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _ExPo, class _FwdIt1, class _DestTy, size_t _DestSize,
          _Enable_if_execution_policy_t<_ExPo> = 0>
inline _DestTy* move(_ExPo&&, _FwdIt1 _First, _FwdIt1 _Last,
                     _DestTy (&_Dest)[_DestSize]) noexcept {
  // move [_First, _Last) to [_Dest, ...)
  // not parallelized as benchmarks show it isn't worth it
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
  return _STD move(_First, _Last, _Dest);
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE move_backward
#if _HAS_IF_CONSTEXPR
template <class _BidIt1, class _BidIt2>
inline _BidIt2 _Move_backward_unchecked(_BidIt1 _First, _BidIt1 _Last,
                                        _BidIt2 _Dest) {
  // move [_First, _Last) backwards to [..., _Dest)
  // note: _Move_backward_unchecked is called directly from elsewhere in the STL
  if constexpr (_Ptr_move_cat<_BidIt1, _BidIt2>::_Trivially_copyable) {
    return _Copy_backward_memmove(_First, _Last, _Dest);
  } else {
    while (_First != _Last) {
      *--_Dest = _STD move(*--_Last);
    }

    return _Dest;
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _BidIt1, class _BidIt2>
inline _BidIt2 _Move_backward_unchecked1(_BidIt1 _First, _BidIt1 _Last,
                                         _BidIt2 _Dest, false_type) {
  // move [_First, _Last) backwards to [..., _Dest), no special optimization
  while (_First != _Last) {
    *--_Dest = _STD move(*--_Last);
  }

  return _Dest;
}

template <class _BidIt1, class _BidIt2>
inline _BidIt2 _Move_backward_unchecked1(_BidIt1 _First, _BidIt1 _Last,
                                         _BidIt2 _Dest, true_type) {
  // move [_First, _Last) backwards to [..., _Dest), memmove optimization
  return _Copy_backward_memmove(_First, _Last, _Dest);
}

template <class _BidIt1, class _BidIt2>
inline _BidIt2 _Move_backward_unchecked(_BidIt1 _First, _BidIt1 _Last,
                                        _BidIt2 _Dest) {
  // move [_First, _Last) backwards to [..., _Dest), choose optimization
  // note: _Move_backward_unchecked is called directly from elsewhere in the STL
  return _Move_backward_unchecked1(
      _First, _Last, _Dest,
      bool_constant<_Ptr_move_cat<_BidIt1, _BidIt2>::_Trivially_copyable>{});
}
#endif  // _HAS_IF_CONSTEXPR

template <class _BidIt1, class _BidIt2>
inline _BidIt2 move_backward(
    _BidIt1 _First, _BidIt1 _Last,
    _BidIt2 _Dest) {  // move [_First, _Last) backwards to [..., _Dest)
  _Adl_verify_range(_First, _Last);
  const auto _UFirst = _Get_unwrapped(_First);
  const auto _ULast = _Get_unwrapped(_Last);
  const auto _UDest =
      _Get_unwrapped_n(_Dest, -_Idl_distance<_BidIt1>(_UFirst, _ULast));
  _Seek_wrapped(_Dest, _Move_backward_unchecked(_UFirst, _ULast, _UDest));
  return _Dest;
}

#if _HAS_CXX17
template <class _ExPo, class _BidIt1, class _BidIt2,
          _Enable_if_execution_policy_t<_ExPo> = 0>
inline _BidIt2 move_backward(_ExPo&&, _BidIt1 _First, _BidIt1 _Last,
                             _BidIt2 _Dest) noexcept {
  // move [_First, _Last) backwards to [..., _Dest)
  // not parallelized as benchmarks show it isn't worth it
  return _STD move_backward(_First, _Last, _Dest);
}
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE fill
template <class _Ty>
struct _Is_character : false_type {};  // by default, not a character type

template <>
struct _Is_character<char> : true_type {};  // chars are characters

template <>
struct _Is_character<signed char> : true_type {
};  // signed chars are also characters

template <>
struct _Is_character<unsigned char> : true_type {
};  // unsigned chars are also characters

#if _HAS_CHAR8_T
template <>
struct _Is_character<char8_t> : true_type {
};      // UTF-8 code units are sort-of characters
#endif  // _HAS_CHAR8_T

template <class _Ty>
struct _Is_character_or_byte : _Is_character<_Ty>::type {};

#if _HAS_STD_BYTE
template <>
struct _Is_character_or_byte<byte> : true_type {};
#endif  // _HAS_STD_BYTE

// _Fill_memset_is_safe determines if _FwdIt and _Ty are eligible for memset
// optimization in fill
// clang-format off
template <class _FwdIt, class _Ty,
    class _Value_type = _Iter_value_t<_FwdIt>,
    class _Raw_ty = _Unwrap_enum_t<_Ty>,
    class _Raw_value_type = _Unwrap_enum_t<_Value_type>>
_INLINE_VAR constexpr bool _Fill_memset_is_safe =
    conjunction_v<
        is_pointer<_FwdIt>,
        disjunction<
            conjunction<_Is_character_or_byte<_Raw_ty>, _Is_character_or_byte<_Raw_value_type>>,
            conjunction<is_same<bool, _Raw_ty>, is_same<bool, _Raw_value_type>>>,
        is_assignable<_Iter_ref_t<_FwdIt>, const _Ty&>>;
// clang-format on

#if _HAS_IF_CONSTEXPR
template <class _FwdIt, class _Ty>
inline void fill(const _FwdIt _First, const _FwdIt _Last,
                 const _Ty& _Val) {  // copy _Val through [_First, _Last)
  _Adl_verify_range(_First, _Last);
  auto _UFirst = _Get_unwrapped(_First);
  const auto _ULast = _Get_unwrapped(_Last);
  if constexpr (_Fill_memset_is_safe<_Unwrapped_t<_FwdIt>, _Ty>) {
    _CSTD memset(_UFirst, static_cast<unsigned char>(_Val),
                 static_cast<size_t>(_ULast - _UFirst));
  } else {
    for (; _UFirst != _ULast; ++_UFirst) {
      *_UFirst = _Val;
    }
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR // !_HAS_IF_CONSTEXPR vvv
template <class _FwdIt, class _Ty>
inline void _Fill_unchecked1(_FwdIt _First, _FwdIt _Last, const _Ty& _Val,
                             false_type) {
  // copy _Val through [_First, _Last), no special optimization
  for (; _First != _Last; ++_First) {
    *_First = _Val;
  }
}

template <class _FwdIt, class _Ty>
inline void _Fill_unchecked1(_FwdIt _First, _FwdIt _Last, const _Ty& _Val,
                             true_type) {
  // copy _Val through [_First, _Last), memset optimization
  _CSTD memset(_First, static_cast<unsigned char>(_Val),
               static_cast<size_t>(_Last - _First));
}

template <class _FwdIt, class _Ty>
inline void fill(_FwdIt _First, _FwdIt _Last,
                 const _Ty& _Val) {  // copy _Val through [_First, _Last)
  _Adl_verify_range(_First, _Last);
  _Fill_unchecked1(
      _Get_unwrapped(_First), _Get_unwrapped(_Last), _Val,
      bool_constant<_Fill_memset_is_safe<_Unwrapped_t<_FwdIt>, _Ty>>{});
}
#endif  // _HAS_IF_CONSTEXPR

#if _HAS_CXX17
template <class _ExPo, class _FwdIt, class _Ty,
          _Enable_if_execution_policy_t<_ExPo> = 0>
inline void fill(_ExPo&&, _FwdIt _First, _FwdIt _Last,
                 const _Ty& _Val) noexcept {
  // copy _Val through [_First, _Last)
  // not parallelized as benchmarks show it isn't worth it
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt);
  return _STD fill(_First, _Last, _Val);
}
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE fill_n
// TRANSITION: _Fill_n_unchecked and _Fill_n_unchecked1 are ABI zombie names
#if _HAS_IF_CONSTEXPR
template <class _OutIt, class _Diff, class _Ty>
inline _OutIt fill_n(_OutIt _Dest, const _Diff _Count_raw, const _Ty& _Val) {
  // copy _Val _Count times through [_Dest, ...)
  _Algorithm_int_t<_Diff> _Count = _Count_raw;
  if (0 < _Count) {
    auto _UDest = _Get_unwrapped_n(_Dest, _Count);
    if constexpr (_Fill_memset_is_safe<decltype(_UDest), _Ty>) {
      _CSTD memset(_UDest, static_cast<unsigned char>(_Val),
                   static_cast<size_t>(_Count));
      _UDest += _Count;
    } else {
      for (; 0 < _Count; --_Count, (void)++_UDest) {
        *_UDest = _Val;
      }
    }

    _Seek_wrapped(_Dest, _UDest);
  }

  return _Dest;
}

#else   // ^^^ _HAS_IF_CONSTEXPR // !_HAS_IF_CONSTEXPR vvv
template <class _OutIt, class _Diff, class _Ty>
inline _OutIt _Fill_n_unchecked2(_OutIt _Dest, _Diff _Count, const _Ty& _Val,
                                 false_type) {
  // copy _Val _Count times through [_Dest, ...), no special optimization
  for (; 0 < _Count; --_Count, (void)++_Dest) {
    *_Dest = _Val;
  }

  return _Dest;
}

template <class _OutIt, class _Diff, class _Ty>
inline _OutIt _Fill_n_unchecked2(_OutIt _Dest, _Diff _Count, const _Ty& _Val,
                                 true_type) {
  // copy _Val _Count times through [_Dest, ...), memset optimization
  _CSTD memset(_Dest, static_cast<unsigned char>(_Val),
               static_cast<size_t>(_Count));
  return _Dest + _Count;
}

template <class _OutIt, class _Diff, class _Ty>
inline _OutIt fill_n(_OutIt _Dest, const _Diff _Count_raw, const _Ty& _Val) {
  // copy _Val _Count times through [_Dest, ...)
  const _Algorithm_int_t<_Diff> _Count = _Count_raw;
  if (0 < _Count) {
    auto _UDest = _Get_unwrapped_n(_Dest, _Count);
    _Seek_wrapped(
        _Dest,
        _Fill_n_unchecked2(
            _UDest, _Count, _Val,
            bool_constant<_Fill_memset_is_safe<decltype(_UDest), _Ty>>{}));
  }

  return _Dest;
}
#endif  // _HAS_IF_CONSTEXPR

#if _HAS_CXX17
template <class _ExPo, class _FwdIt, class _Diff, class _Ty,
          _Enable_if_execution_policy_t<_ExPo> = 0>
inline _FwdIt fill_n(_ExPo&&, _FwdIt _Dest, _Diff _Count_raw,
                     const _Ty& _Val) noexcept {
  // copy _Val _Count times through [_Dest, ...)
  // not parallelized as benchmarks show it isn't worth it
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt);
  return _STD fill_n(_Dest, _Count_raw, _Val);
}
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE equal
template <class _Elem1, class _Elem2>
struct _Value_equality_is_bitwise_equality
    : bool_constant<static_cast<_Elem1>(-1) == static_cast<_Elem2>(-1)> {
  // Tests whether the usual arithmetic conversions will preserve the
  // bit-pattern when promoting to int e.g. short == unsigned short -> false
  //      int == unsigned int -> true
};

template <class _Elem1, class _Elem2,
          class _Pr>
struct _Equal_memcmp_is_safe_helper
    : false_type {  // determines whether it is safe to call memcmp to compare
                    // things; defaults to false
};

template <class _Elem1, class _Elem2>
struct _Equal_memcmp_is_safe_helper<_Elem1, _Elem2, equal_to<>>
    : bool_constant<conjunction_v<
          bool_constant<sizeof(_Elem1) == sizeof(_Elem2) &&
                        _Is_nonbool_integral<_Elem1> &&
                        _Is_nonbool_integral<_Elem2>>,
          negation<is_volatile<_Elem1>>, negation<is_volatile<_Elem2>>,
          // note that order matters here, as being integral is a precondition
          // of _Value_equality_is_bitwise_equality
          _Value_equality_is_bitwise_equality<
              _Elem1, _Elem2>>> {  // allow memcmping same-size integral
                                   // non-bool non-volatile bitwise types using
                                   // equal_to<>
};

template <class _Elem1, class _Elem2>
struct _Equal_memcmp_is_safe_helper<_Elem1*, _Elem2*, equal_to<>>
    : is_same<remove_cv_t<_Elem1>, remove_cv_t<_Elem2>>::
          type {  // allow memcmping pointers-to-cv-T with equal_to<>
};

#if _HAS_STD_BYTE
template <>
struct _Equal_memcmp_is_safe_helper<byte, byte, equal_to<>>
    : true_type {  // allow memcmping std::byte
};
#endif  // _HAS_STD_BYTE

template <class _Elem>
struct _Equal_memcmp_is_safe_helper<_Elem, _Elem, equal_to<_Elem>>
    : _Equal_memcmp_is_safe_helper<_Elem, _Elem, equal_to<>>::
          type {  // treat equal_to with exact T as equal_to<>
                  // this is safe because we only activate the
                  // optimization for builtin _Elem (and std::byte)
};

template <class _Iter1, class _Iter2, class _Pr>
inline false_type _Equal_memcmp_is_safe(
    const _Iter1&, const _Iter2&,
    const _Pr&) {  // return equal optimization category for arbitrary iterators
  return {};
}

template <class _Obj1, class _Obj2, class _Pr>
inline typename _Equal_memcmp_is_safe_helper<remove_const_t<_Obj1>,
                                             remove_const_t<_Obj2>, _Pr>::type
_Equal_memcmp_is_safe(
    _Obj1* const&, _Obj2* const&,
    const _Pr&) {  // return equal optimization category for pointers
  return {};
}

#if _HAS_IF_CONSTEXPR
template <class _InIt1, class _InIt2, class _Pr>
_NODISCARD inline bool equal(const _InIt1 _First1, const _InIt1 _Last1,
                             const _InIt2 _First2, _Pr _Pred) {
  // compare [_First1, _Last1) to [_First2, ...) using _Pred
  _Adl_verify_range(_First1, _Last1);
  auto _UFirst1 = _Get_unwrapped(_First1);
  const auto _ULast1 = _Get_unwrapped(_Last1);
  auto _UFirst2 =
      _Get_unwrapped_n(_First2, _Idl_distance<_InIt1>(_UFirst1, _ULast1));
  if constexpr (decltype(
                    _Equal_memcmp_is_safe(_UFirst1, _UFirst2, _Pred))::value) {
    const auto _First1_ch = reinterpret_cast<const char*>(_UFirst1);
    const auto _First2_ch = reinterpret_cast<const char*>(_UFirst2);
    const auto _Count = static_cast<size_t>(
        reinterpret_cast<const char*>(_ULast1) - _First1_ch);
    return _CSTD memcmp(_First1_ch, _First2_ch, _Count) == 0;
  } else {
    for (; _UFirst1 != _ULast1; ++_UFirst1, (void)++_UFirst2) {
      if (!_Pred(*_UFirst1, *_UFirst2)) {
        return false;
      }
    }

    return true;
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _InIt1, class _InIt2, class _Pr>
inline bool _Equal_unchecked1(_InIt1 _First1, const _InIt1 _Last1,
                              _InIt2 _First2, _Pr _Pred, false_type) {
  // compare [_First1, _Last1) to [_First2, ...) using _Pred, no special
  // optimization
  for (; _First1 != _Last1; ++_First1, (void)++_First2) {
    if (!_Pred(*_First1, *_First2)) {
      return false;
    }
  }

  return true;
}

template <class _InIt1, class _InIt2, class _Pr>
inline bool _Equal_unchecked1(const _InIt1 _First1, const _InIt1 _Last1,
                              const _InIt2 _First2, _Pr, true_type) {
  // compare [_First1, _Last1) to [_First2, ...), memcmp optimization
  const auto _First1_ch = reinterpret_cast<const char*>(_First1);
  const auto _First2_ch = reinterpret_cast<const char*>(_First2);
  const auto _Count =
      static_cast<size_t>(reinterpret_cast<const char*>(_Last1) - _First1_ch);
  return _CSTD memcmp(_First1_ch, _First2_ch, _Count) == 0;
}

template <class _InIt1, class _InIt2, class _Pr>
inline bool _Equal_unchecked(const _InIt1 _First1, const _InIt1 _Last1,
                             const _InIt2 _First2, _Pr _Pred) {
  // compare [_First1, _Last1) to [_First2, ...) using _Pred, choose
  // optimization
  return _Equal_unchecked1(_First1, _Last1, _First2, _Pred,
                           _Equal_memcmp_is_safe(_First1, _First2, _Pred));
}

template <class _InIt1, class _InIt2, class _Pr>
_NODISCARD inline bool equal(const _InIt1 _First1, const _InIt1 _Last1,
                             const _InIt2 _First2, _Pr _Pred) {
  // compare [_First1, _Last1) to [_First2, ...) using _Pred
  _Adl_verify_range(_First1, _Last1);
  const auto _UFirst1 = _Get_unwrapped(_First1);
  const auto _ULast1 = _Get_unwrapped(_Last1);
  const auto _UFirst2 =
      _Get_unwrapped_n(_First2, _Idl_distance<_InIt1>(_UFirst1, _ULast1));
  return _Equal_unchecked(_UFirst1, _ULast1, _UFirst2, _Pass_fn(_Pred));
}
#endif  // _HAS_IF_CONSTEXPR

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _InIt1, class _RightTy, size_t _RightSize, class _Pr,
          enable_if_t<!is_same_v<_RightTy*, _Pr>, int> = 0>
_NODISCARD inline bool equal(const _InIt1 _First1, const _InIt1 _Last1,
                             _RightTy (&_First2)[_RightSize], _Pr _Pred) {
  // compare [_First1, _Last1) to [_First2, ...) using _Pred
  return _STD equal(_First1, _Last1,
                    _Array_iterator<_RightTy, _RightSize>(_First2),
                    _Pass_fn(_Pred));
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS

#if _HAS_CXX17
template <class _ExPo, class _FwdIt1, class _FwdIt2, class _Pr,
          _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD inline bool equal(_ExPo&& _Exec, const _FwdIt1 _First1,
                             const _FwdIt1 _Last1, const _FwdIt2 _First2,
                             _Pr _Pred) noexcept;

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _ExPo, class _FwdIt1, class _RightTy, size_t _RightSize,
          class _Pr,
          enable_if_t<is_execution_policy_v<decay_t<_ExPo>> &&
                          !is_same_v<_RightTy*, _Pr>,
                      int> /* = 0 */>
_NODISCARD inline bool equal(_ExPo&& _Exec, _FwdIt1 _First1, _FwdIt1 _Last1,
                             _RightTy (&_First2)[_RightSize],
                             _Pr _Pred) noexcept {
  // compare [_First1, _Last1) to [_First2, ...) using _Pred
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
  _Adl_verify_range(_First1, _Last1);
  return _STD equal(_STD forward<_ExPo>(_Exec), _Get_unwrapped(_First1),
                    _Get_unwrapped(_Last1),
                    _Array_iterator<_RightTy, _RightSize>(_First2),
                    _Pass_fn(_Pred));
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS
#endif  // _HAS_CXX17

template <class _InIt1, class _InIt2>
_NODISCARD inline bool equal(const _InIt1 _First1, const _InIt1 _Last1,
                             const _InIt2 _First2) {
  // compare [_First1, _Last1) to [_First2, ...)
  return _STD equal(_First1, _Last1, _First2, equal_to<>());
}

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _InIt1, class _RightTy, size_t _RightSize>
_NODISCARD inline bool equal(const _InIt1 _First1, const _InIt1 _Last1,
                             _RightTy (&_First2)[_RightSize]) {
  // compare [_First1, _Last1) to [_First2, ...)
  return _STD equal(_First1, _Last1, _First2, equal_to<>());
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS

#if _HAS_CXX17
template <class _ExPo, class _FwdIt1, class _FwdIt2,
          _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD inline bool equal(_ExPo&& _Exec, const _FwdIt1 _First1,
                             const _FwdIt1 _Last1,
                             const _FwdIt2 _First2) noexcept {
  // compare [_First1, _Last1) to [_First2, ...)
  return _STD equal(_STD forward<_ExPo>(_Exec), _First1, _Last1, _First2,
                    equal_to<>());
}

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _ExPo, class _FwdIt1, class _RightTy, size_t _RightSize,
          _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD inline bool equal(_ExPo&& _Exec, const _FwdIt1 _First1,
                             const _FwdIt1 _Last1,
                             _RightTy (&_First2)[_RightSize]) noexcept {
  // compare [_First1, _Last1) to [_First2, ...)
  return _STD equal(_STD forward<_ExPo>(_Exec), _First1, _Last1, _First2,
                    equal_to<>());
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS
#endif  // _HAS_CXX17

#if _HAS_IF_CONSTEXPR
template <class _InIt1, class _InIt2, class _Pr>
_NODISCARD inline bool equal(const _InIt1 _First1, const _InIt1 _Last1,
                             const _InIt2 _First2, const _InIt2 _Last2,
                             _Pr _Pred) {
  // compare [_First1, _Last1) to [_First2, _Last2) using _Pred
  _Adl_verify_range(_First1, _Last1);
  _Adl_verify_range(_First2, _Last2);
  auto _UFirst1 = _Get_unwrapped(_First1);
  const auto _ULast1 = _Get_unwrapped(_Last1);
  auto _UFirst2 = _Get_unwrapped(_First2);
  const auto _ULast2 = _Get_unwrapped(_Last2);
  if constexpr (_Is_random_iter_v<_InIt1> && _Is_random_iter_v<_InIt2>) {
    if (_ULast1 - _UFirst1 != _ULast2 - _UFirst2) {
      return false;
    }

    return _STD equal(_UFirst1, _ULast1, _UFirst2, _Pass_fn(_Pred));
  } else {
    for (;;) {
      if (_UFirst1 == _ULast1) {
        return _UFirst2 == _ULast2;
      }

      if (_UFirst2 == _ULast2) {
        return false;
      }

      if (!_Pred(*_UFirst1, *_UFirst2)) {
        return false;
      }

      ++_UFirst1;
      ++_UFirst2;
    }
  }
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _InIt1, class _InIt2, class _Pr>
inline bool _Equal_unchecked(_InIt1 _First1, const _InIt1 _Last1,
                             _InIt2 _First2, const _InIt2 _Last2, _Pr _Pred,
                             input_iterator_tag, input_iterator_tag) {
  // compare [_First1, _Last1) to [_First2, _Last2) using _Pred, arbitrary
  // iterators
  for (;;) {
    if (_First1 == _Last1) {
      return _First2 == _Last2;
    }

    if (_First2 == _Last2) {
      return false;
    }

    if (!_Pred(*_First1, *_First2)) {
      return false;
    }

    ++_First1;
    ++_First2;
  }
}

template <class _InIt1, class _InIt2, class _Pr>
inline bool _Equal_unchecked(const _InIt1 _First1, const _InIt1 _Last1,
                             const _InIt2 _First2, const _InIt2 _Last2,
                             _Pr _Pred, random_access_iterator_tag,
                             random_access_iterator_tag) {
  // compare [_First1, _Last1) to [_First2, _Last2) using _Pred, random-access
  // iterators
  if (_Last1 - _First1 != _Last2 - _First2) {
    return false;
  }

  return _Equal_unchecked(_First1, _Last1, _First2, _Pred);
}

template <class _InIt1, class _InIt2, class _Pr>
_NODISCARD inline bool equal(const _InIt1 _First1, const _InIt1 _Last1,
                             const _InIt2 _First2, const _InIt2 _Last2,
                             _Pr _Pred) {
  // compare [_First1, _Last1) to [_First2, _Last2) using _Pred
  _Adl_verify_range(_First1, _Last1);
  _Adl_verify_range(_First2, _Last2);
  return _Equal_unchecked(_Get_unwrapped(_First1), _Get_unwrapped(_Last1),
                          _Get_unwrapped(_First2), _Get_unwrapped(_Last2),
                          _Pass_fn(_Pred), _Iter_cat_t<_InIt1>(),
                          _Iter_cat_t<_InIt2>());
}
#endif  // _HAS_IF_CONSTEXPR

#if _HAS_CXX17
template <class _ExPo, class _FwdIt1, class _FwdIt2, class _Pr,
          _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD inline bool equal(_ExPo&& _Exec, const _FwdIt1 _First1,
                             const _FwdIt1 _Last1, const _FwdIt2 _First2,
                             const _FwdIt2 _Last2, _Pr _Pred) noexcept;
#endif  // _HAS_CXX17

template <class _InIt1, class _InIt2>
_NODISCARD inline bool equal(const _InIt1 _First1, const _InIt1 _Last1,
                             const _InIt2 _First2, const _InIt2 _Last2) {
  // compare [_First1, _Last1) to [_First2, _Last2)
  return _STD equal(_First1, _Last1, _First2, _Last2, equal_to<>());
}

#if _HAS_CXX17
template <class _ExPo, class _FwdIt1, class _FwdIt2,
          _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD inline bool equal(_ExPo&& _Exec, const _FwdIt1 _First1,
                             const _FwdIt1 _Last1, const _FwdIt2 _First2,
                             const _FwdIt2 _Last2) noexcept {
  // compare [_First1, _Last1) to [_First2, _Last2)
  return _STD equal(_STD forward<_ExPo>(_Exec), _First1, _Last1, _First2,
                    _Last2, equal_to<>());
}
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE lexicographical_compare
template <class _Elem1, class _Elem2, class _FTy>
struct _Lex_compare_check_element_types_helper
    : bool_constant<conjunction_v<
          _Is_character<_Elem1>, _Is_character<_Elem2>, _Is_character<_FTy>,
          is_unsigned<_FTy>>> {  // checks the lex_compare element types for
                                 // memcmp safety for builtin functors (e.g.
                                 // less<unsigned char>)
};

template <class _Elem1, class _Elem2>
struct _Lex_compare_check_element_types_helper<_Elem1, _Elem2, void>
    : bool_constant<conjunction_v<_Is_character<_Elem1>, _Is_character<_Elem2>,
                                  is_unsigned<_Elem1>, is_unsigned<_Elem2>>> {
  // checks the lex_compare element types for memcmp safety for transparent
  // functors (e.g. less<>)
};

#if _HAS_STD_BYTE
template <>
struct _Lex_compare_check_element_types_helper<byte, byte, byte>
    : true_type {  // std::byte with builtin functors (e.g. less<byte>) is
                   // memcmp safe
};

template <>
struct _Lex_compare_check_element_types_helper<byte, byte, void>
    : true_type {  // std::byte with transparent functors (e.g. less<>) is
                   // memcmp safe
};
#endif  // _HAS_STD_BYTE

template <class _Memcmp_pr>
struct _Lex_compare_optimize {
};  // optimization tag for lexicographical_compare

template <class _Memcmp_pr, class _Obj1, class _Obj2, class _FTy>
using _Lex_compare_check_element_types = _Lex_compare_optimize<conditional_t<
    _Lex_compare_check_element_types_helper<remove_const_t<_Obj1>,
                                            remove_const_t<_Obj2>, _FTy>::value,
    _Memcmp_pr, void>>;  // checks the lex_compare element types for memcmp
                         // safety

template <class _InIt1, class _InIt2, class _Pr>
inline _Lex_compare_optimize<void> _Lex_compare_memcmp_classify(const _InIt1&,
                                                                const _InIt2&,
                                                                const _Pr&) {
  // return lex_compare optimization category for arbitrary iterators
  return {};
}

template <class _Obj1, class _Obj2, class _FTy>
inline _Lex_compare_check_element_types<less<int>, _Obj1, _Obj2, _FTy>
_Lex_compare_memcmp_classify(_Obj1* const&, _Obj2* const&, const less<_FTy>&) {
  // return lex_compare optimization category for pointer iterators and
  // less<_FTy>
  return {};
}

template <class _Obj1, class _Obj2, class _FTy>
inline _Lex_compare_check_element_types<greater<int>, _Obj1, _Obj2, _FTy>
_Lex_compare_memcmp_classify(_Obj1* const&, _Obj2* const&,
                             const greater<_FTy>&) {
  // return lex_compare optimization category for pointer iterators and
  // greater<_FTy>
  return {};
}

template <class _InIt1, class _InIt2, class _Pr>
inline bool _Lex_compare_unchecked(_InIt1 _First1, _InIt1 _Last1,
                                   _InIt2 _First2, _InIt2 _Last2, _Pr _Pred,
                                   _Lex_compare_optimize<void>) {
  // order [_First1, _Last1) vs. [_First2, _Last2) using _Pred, no special
  // optimization
  for (; _First1 != _Last1 && _First2 != _Last2;
       ++_First1, (void)++_First2) {  // something to compare, do it
    if (_DEBUG_LT_PRED(_Pred, *_First1, *_First2)) {
      return true;
    } else if (_Pred(*_First2, *_First1)) {
      return false;
    }
  }

  return _First1 == _Last1 && _First2 != _Last2;
}

template <class _InIt1, class _InIt2, class _Pr, class _Memcmp_pr>
inline bool _Lex_compare_unchecked(_InIt1 _First1, _InIt1 _Last1,
                                   _InIt2 _First2, _InIt2 _Last2, _Pr,
                                   _Lex_compare_optimize<_Memcmp_pr>) {
  // order [_First1, _Last1) vs. [_First2, _Last2) memcmp optimization
  const auto _Num1 = static_cast<size_t>(_Last1 - _First1);
  const auto _Num2 = static_cast<size_t>(_Last2 - _First2);
  const int _Ans =
      _CSTD memcmp(_First1, _First2, _Num1 < _Num2 ? _Num1 : _Num2);
  return _Memcmp_pr{}(_Ans, 0) || (_Ans == 0 && _Num1 < _Num2);
}

template <class _InIt1, class _InIt2, class _Pr>
_NODISCARD inline bool lexicographical_compare(_InIt1 _First1, _InIt1 _Last1,
                                               _InIt2 _First2, _InIt2 _Last2,
                                               _Pr _Pred) {
  // order [_First1, _Last1) vs. [_First2, _Last2) using _Pred
  _Adl_verify_range(_First1, _Last1);
  _Adl_verify_range(_First2, _Last2);
  const auto _UFirst1 = _Get_unwrapped(_First1);
  const auto _ULast1 = _Get_unwrapped(_Last1);
  const auto _UFirst2 = _Get_unwrapped(_First2);
  const auto _ULast2 = _Get_unwrapped(_Last2);
  return _Lex_compare_unchecked(
      _UFirst1, _ULast1, _UFirst2, _ULast2, _Pass_fn(_Pred),
      _Lex_compare_memcmp_classify(_UFirst1, _UFirst2, _Pred));
}

template <class _InIt1, class _InIt2>
_NODISCARD inline bool lexicographical_compare(_InIt1 _First1, _InIt1 _Last1,
                                               _InIt2 _First2, _InIt2 _Last2) {
  // order [_First1, _Last1) vs. [_First2, _Last2)
  return _STD lexicographical_compare(_First1, _Last1, _First2, _Last2,
                                      less<>());
}

#if _HAS_CXX17
template <class _ExPo, class _FwdIt1, class _FwdIt2, class _Pr,
          _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD inline bool lexicographical_compare(_ExPo&&, _FwdIt1 _First1,
                                               _FwdIt1 _Last1, _FwdIt2 _First2,
                                               _FwdIt2 _Last2,
                                               _Pr _Pred) noexcept {
  // order [_First1, _Last1) vs. [_First2, _Last2) using _Pred
  // not parallelized at present, parallelism expected to be feasible in a
  // future release
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt2);
  return _STD lexicographical_compare(_First1, _Last1, _First2, _Last2,
                                      _Pass_fn(_Pred));
}

template <class _ExPo, class _FwdIt1, class _FwdIt2,
          _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD inline bool lexicographical_compare(_ExPo&&, _FwdIt1 _First1,
                                               _FwdIt1 _Last1, _FwdIt2 _First2,
                                               _FwdIt2 _Last2) noexcept {
  // order [_First1, _Last1) vs. [_First2, _Last2)
  // not parallelized at present, parallelism expected to be feasible in a
  // future release
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt1);
  _REQUIRE_PARALLEL_ITERATOR(_FwdIt2);
  return _STD lexicographical_compare(_First1, _Last1, _First2, _Last2);
}
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE find
template <class _Ty>
inline bool _Within_limits(const _Ty& _Val, true_type, true_type,
                           _Any_tag) {  // signed _Elem, signed _Ty
  return SCHAR_MIN <= _Val && _Val <= SCHAR_MAX;
}

template <class _Ty>
inline bool _Within_limits(const _Ty& _Val, true_type, false_type, true_type) {
  // signed _Elem, unsigned _Ty, -1 == static_cast<_Ty>(-1)
  return _Val <= SCHAR_MAX || static_cast<_Ty>(SCHAR_MIN) <= _Val;
}

template <class _Ty>
inline bool _Within_limits(const _Ty& _Val, true_type, false_type, false_type) {
  // signed _Elem, unsigned _Ty, -1 != static_cast<_Ty>(-1)
  return _Val <= SCHAR_MAX;
}

template <class _Ty>
inline bool _Within_limits(const _Ty& _Val, false_type, true_type, _Any_tag) {
  // unsigned _Elem, signed _Ty
  return 0 <= _Val && _Val <= UCHAR_MAX;
}

template <class _Ty>
inline bool _Within_limits(const _Ty& _Val, false_type, false_type, _Any_tag) {
  // unsigned _Elem, unsigned _Ty
  return _Val <= UCHAR_MAX;
}

template <class _InIt, class _Ty>
inline bool _Within_limits(
    _InIt,
    const _Ty& _Val) {  // check whether _Val is within the limits of _Elem
  using _Elem = remove_pointer_t<_InIt>;
  return _Within_limits(_Val, is_signed<_Elem>{}, is_signed<_Ty>{},
                        bool_constant<-1 == static_cast<_Ty>(-1)>{});
}

template <class _InIt>
inline bool _Within_limits(
    _InIt, const bool&) {  // bools are always within the limits of _Elem
  return true;
}

template <class _InIt, class _Ty>
inline _InIt _Find_unchecked1(_InIt _First, const _InIt _Last, const _Ty& _Val,
                              true_type) {
  // find first byte matching integral _Val
  if (!_Within_limits(_First, _Val)) {
    return _Last;
  }

  _First =
      static_cast<_InIt>(_CSTD memchr(_First, static_cast<unsigned char>(_Val),
                                      static_cast<size_t>(_Last - _First)));
  return _First ? _First : _Last;
}

template <class _InIt, class _Ty>
inline _InIt _Find_unchecked1(_InIt _First, const _InIt _Last, const _Ty& _Val,
                              false_type) {
  // find first matching _Val
  for (; _First != _Last; ++_First) {
    if (*_First == _Val) {
      break;
    }
  }

  return _First;
}

template <class _InIt, class _Ty>
inline _InIt _Find_unchecked(const _InIt _First, const _InIt _Last,
                             const _Ty& _Val) {
  // find first matching _Val; choose optimization
  // activate optimization for pointers to (const) bytes and integral values
  using _Memchr_opt = bool_constant<
      is_integral_v<_Ty> &&
      _Is_any_of_v<_InIt, char*, signed char*, unsigned char*,  //
                   const char*, const signed char*, const unsigned char*>>;

  return _Find_unchecked1(_First, _Last, _Val, _Memchr_opt{});
}

template <class _InIt, class _Ty>
_NODISCARD inline _InIt find(_InIt _First, const _InIt _Last,
                             const _Ty& _Val) {  // find first matching _Val
  _Adl_verify_range(_First, _Last);
  _Seek_wrapped(_First, _Find_unchecked(_Get_unwrapped(_First),
                                        _Get_unwrapped(_Last), _Val));
  return _First;
}

#if _HAS_CXX17
template <class _ExPo, class _FwdIt, class _Ty,
          _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD inline _FwdIt find(_ExPo&& _Exec, _FwdIt _First, const _FwdIt _Last,
                              const _Ty& _Val) noexcept;
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE _Find_pr
template <class _InIt, class _Ty, class _Pr>
inline _InIt _Find_pr(_InIt _First, _InIt _Last, const _Ty& _Val,
                      _Pr _Pred) {  // find first matching _Val, using _Pred
  for (; _First != _Last; ++_First) {
    if (_Pred(*_First, _Val)) {
      break;
    }
  }

  return _First;
}

// FUNCTION TEMPLATE count
template <class _InIt, class _Ty>
_NODISCARD inline _Iter_diff_t<_InIt> count(const _InIt _First,
                                            const _InIt _Last,
                                            const _Ty& _Val) {
  // count elements that match _Val
  _Adl_verify_range(_First, _Last);
  auto _UFirst = _Get_unwrapped(_First);
  const auto _ULast = _Get_unwrapped(_Last);
  _Iter_diff_t<_InIt> _Count = 0;

  for (; _UFirst != _ULast; ++_UFirst) {
    if (*_UFirst == _Val) {
      ++_Count;
    }
  }

  return _Count;
}

#if _HAS_CXX17
template <class _ExPo, class _FwdIt, class _Ty,
          _Enable_if_execution_policy_t<_ExPo> = 0>
_NODISCARD inline _Iter_diff_t<_FwdIt> count(_ExPo&& _Exec, const _FwdIt _First,
                                             const _FwdIt _Last,
                                             const _Ty& _Val) noexcept;
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE _Count_pr
template <class _InIt, class _Ty, class _Pr>
inline _Iter_diff_t<_InIt> _Count_pr(_InIt _First, _InIt _Last, const _Ty& _Val,
                                     _Pr _Pred) {
  // count elements that match _Val, using _Pred
  _Iter_diff_t<_InIt> _Count = 0;

  for (; _First != _Last; ++_First) {
    if (_Pred(*_First, _Val)) {
      ++_Count;
    }
  }

  return _Count;
}

// FUNCTION TEMPLATE _Trim_matching_suffixes
template <class _FwdIt1, class _FwdIt2, class _Pr>
inline void _Trim_matching_suffixes(_FwdIt1&, _FwdIt2&, _Pr,
                                    forward_iterator_tag,
                                    forward_iterator_tag) {
  // trim matching suffixes, forward iterators (do nothing)
}

template <class _FwdIt1, class _FwdIt2, class _Pr>
inline void _Trim_matching_suffixes(_FwdIt1& _Last1, _FwdIt2& _Last2, _Pr _Pred,
                                    bidirectional_iterator_tag,
                                    bidirectional_iterator_tag) {
  // trim matching suffixes, bidirectional iterators
  // assumptions: same lengths, non-empty, !_Pred(*_First1, *_First2)
  do {  // find last inequality
    --_Last1;
    --_Last2;
  } while (_Pred(*_Last1, *_Last2));
  ++_Last1;
  ++_Last2;
}

// FUNCTION TEMPLATE _Check_match_counts
template <class _FwdIt1, class _FwdIt2, class _Pr>
inline bool _Check_match_counts(_FwdIt1 _First1, _FwdIt1 _Last1,
                                _FwdIt2 _First2, _FwdIt2 _Last2, _Pr _Pred) {
  // test if [_First1, _Last1) == permuted [_First2, _Last2), using _Pred, same
  // lengths
  _Trim_matching_suffixes(_Last1, _Last2, _Pred, _Iter_cat_t<_FwdIt1>(),
                          _Iter_cat_t<_FwdIt2>());
  for (_FwdIt1 _Next1 = _First1; _Next1 != _Last1; ++_Next1) {
    if (_Next1 == _Find_pr(_First1, _Next1, *_Next1,
                           _Pred)) {  // new value, compare match counts
      _Iter_diff_t<_FwdIt2> _Count2 =
          _Count_pr(_First2, _Last2, *_Next1, _Pred);
      if (_Count2 == 0) {
        return false;  // second range lacks value, fail
      }

      _FwdIt1 _Skip1 = _Next_iter(_Next1);
      _Iter_diff_t<_FwdIt1> _Count1 =
          _Count_pr(_Skip1, _Last1, *_Next1, _Pred) + 1;
      if (_Count2 != _Count1) {
        return false;  // match counts differ, fail
      }
    }
  }

  return true;
}

// FUNCTION TEMPLATE is_permutation
template <class _FwdIt1, class _FwdIt2, class _Pr>
inline bool _Is_permutation_unchecked(_FwdIt1 _First1, _FwdIt1 _Last1,
                                      _FwdIt2 _First2, _Pr _Pred) {
  // test if [_First1, _Last1) == permuted [_First2, ...), using _Pred
  for (; _First1 != _Last1; ++_First1, (void)++_First2) {
    if (!_Pred(*_First1, *_First2)) {
      // found first inequality, check match counts in suffix narrowing
      // _Iter_diff_t<_FwdIt1> to _Iter_diff_t<_FwdIt2> is OK because if the 2nd
      // range is shorter than the 1st, the user already triggered UB
      auto _Last2 = _STD next(_First2, static_cast<_Iter_diff_t<_FwdIt2>>(
                                           _STD distance(_First1, _Last1)));
      return _Check_match_counts(_First1, _Last1, _First2, _Last2, _Pred);
    }
  }

  return true;
}

template <class _FwdIt1, class _FwdIt2, class _Pr>
_NODISCARD inline bool is_permutation(_FwdIt1 _First1, _FwdIt1 _Last1,
                                      _FwdIt2 _First2, _Pr _Pred) {
  // test if [_First1, _Last1) == permuted [_First2, ...), using _Pred
  _Adl_verify_range(_First1, _Last1);
  const auto _UFirst1 = _Get_unwrapped(_First1);
  const auto _ULast1 = _Get_unwrapped(_Last1);
  const auto _UFirst2 =
      _Get_unwrapped_n(_First2, _Idl_distance<_FwdIt1>(_UFirst1, _ULast1));
  return _Is_permutation_unchecked(_UFirst1, _ULast1, _UFirst2,
                                   _Pass_fn(_Pred));
}

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _FwdIt1, class _RightTy, size_t _RightSize, class _Pr,
          class = enable_if_t<!is_same_v<_RightTy*, _Pr>>>
_NODISCARD inline bool is_permutation(_FwdIt1 _First1, _FwdIt1 _Last1,
                                      _RightTy (&_First2)[_RightSize],
                                      _Pr _Pred) {
  // test if [_First1, _Last1) == permuted [_First2, ...), using _Pred
  return _STD is_permutation(_First1, _Last1,
                             _Array_iterator<_RightTy, _RightSize>(_First2),
                             _Pass_fn(_Pred));
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS

template <class _FwdIt1, class _FwdIt2>
inline bool is_permutation(_FwdIt1 _First1, _FwdIt1 _Last1, _FwdIt2 _First2) {
  // test if [_First1, _Last1) == permuted [_First2, ...)
  return _STD is_permutation(_First1, _Last1, _First2, equal_to<>());
}

#if _ITERATOR_DEBUG_ARRAY_OVERLOADS
template <class _FwdIt1, class _RightTy, size_t _RightSize>
_NODISCARD inline bool is_permutation(_FwdIt1 _First1, _FwdIt1 _Last1,
                                      _RightTy (&_First2)[_RightSize]) {
  // test if [_First1, _Last1) == permuted [_First2, ...)
  return _STD is_permutation(_First1, _Last1, _First2, equal_to<>());
}
#endif  // _ITERATOR_DEBUG_ARRAY_OVERLOADS

template <class _FwdIt1, class _FwdIt2, class _Pr>
inline bool _Is_permutation_unchecked(_FwdIt1 _First1, _FwdIt1 _Last1,
                                      _FwdIt2 _First2, _FwdIt2 _Last2,
                                      _Pr _Pred, forward_iterator_tag,
                                      forward_iterator_tag) {
  // test if [_First1, _Last1) == permuted [_First2, _Last2), using _Pred,
  // arbitrary iterators
  for (; _First1 != _Last1 && _First2 != _Last2; ++_First1, (void)++_First2) {
    if (!_Pred(*_First1, *_First2)) {  // found first inequality, check match
                                       // counts in suffix
      if (_STD distance(_First1, _Last1) == _STD distance(_First2, _Last2)) {
        return _Check_match_counts(_First1, _Last1, _First2, _Last2, _Pred);
      } else {
        return false;  // lengths differ, fail
      }
    }
  }

  return _First1 == _Last1 && _First2 == _Last2;
}

template <class _FwdIt1, class _FwdIt2, class _Pr>
inline bool _Is_permutation_unchecked(_FwdIt1 _First1, _FwdIt1 _Last1,
                                      _FwdIt2 _First2, _FwdIt2 _Last2,
                                      _Pr _Pred, random_access_iterator_tag,
                                      random_access_iterator_tag) {
  // test if [_First1, _Last1) == permuted [_First2, _Last2), using _Pred,
  // random-access iterators
  if (_Last1 - _First1 != _Last2 - _First2) {
    return false;
  }

  return _Is_permutation_unchecked(_First1, _Last1, _First2, _Pred);
}

template <class _FwdIt1, class _FwdIt2, class _Pr>
_NODISCARD inline bool is_permutation(_FwdIt1 _First1, _FwdIt1 _Last1,
                                      _FwdIt2 _First2, _FwdIt2 _Last2,
                                      _Pr _Pred) {
  // test if [_First1, _Last1) == permuted [_First2, _Last2), using _Pred
  _Adl_verify_range(_First1, _Last1);
  _Adl_verify_range(_First2, _Last2);
  return _Is_permutation_unchecked(
      _Get_unwrapped(_First1), _Get_unwrapped(_Last1), _Get_unwrapped(_First2),
      _Get_unwrapped(_Last2), _Pass_fn(_Pred), _Iter_cat_t<_FwdIt1>(),
      _Iter_cat_t<_FwdIt2>());
}

// FUNCTION TEMPLATE is_permutation WITH TWO RANGES
template <class _FwdIt1, class _FwdIt2>
_NODISCARD inline bool is_permutation(_FwdIt1 _First1, _FwdIt1 _Last1,
                                      _FwdIt2 _First2, _FwdIt2 _Last2) {
  // test if [_First1, _Last1) == permuted [_First2, _Last2)
  return _STD is_permutation(_First1, _Last1, _First2, _Last2, equal_to<>());
}

// FUNCTION TEMPLATE reverse
#if _HAS_IF_CONSTEXPR
template <class _BidIt>
inline void reverse(
    const _BidIt _First,
    const _BidIt _Last) {  // reverse elements in [_First, _Last)
  _Adl_verify_range(_First, _Last);
  auto _UFirst = _Get_unwrapped(_First);
  auto _ULast = _Get_unwrapped(_Last);
#if (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_CEE_PURE) && \
    !defined(_M_HYBRID)
  using _Elem = remove_pointer_t<decltype(_UFirst)>;
  constexpr bool _Allow_vectorization =
      conjunction_v<is_pointer<decltype(_UFirst)>,
                    _Is_trivially_swappable<_Elem>,
                    negation<is_volatile<_Elem>>>;
  if constexpr (_Allow_vectorization && sizeof(_Elem) == 1) {
    __std_reverse_trivially_swappable_1(_UFirst, _ULast);
  } else if constexpr (_Allow_vectorization && sizeof(_Elem) == 2) {
    __std_reverse_trivially_swappable_2(_UFirst, _ULast);
  } else if constexpr (_Allow_vectorization && sizeof(_Elem) == 4) {
    __std_reverse_trivially_swappable_4(_UFirst, _ULast);
  } else if constexpr (_Allow_vectorization && sizeof(_Elem) == 8) {
    __std_reverse_trivially_swappable_8(_UFirst, _ULast);
  } else
#endif  // (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_CEE_PURE) &&
        // !defined(_M_HYBRID)
  {
    for (; _UFirst != _ULast && _UFirst != --_ULast; ++_UFirst) {
      _STD iter_swap(_UFirst, _ULast);
    }
  }
}
#else  // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _BidIt>
inline void _Reverse_unchecked1(_BidIt _First, _BidIt _Last,
                                integral_constant<size_t, 0>) {
  // reverse elements in [_First, _Last), general bidirectional iterators
  for (; _First != _Last && _First != --_Last; ++_First) {
    _STD iter_swap(_First, _Last);
  }
}

#if (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_CEE_PURE) && \
    !defined(_M_HYBRID)
template <class _BidIt>
inline void _Reverse_unchecked1(const _BidIt _First, const _BidIt _Last,
                                integral_constant<size_t, 1>) {
  // reverse elements in [_First, _Last), pointers to trivially swappable of
  // size 1
  __std_reverse_trivially_swappable_1(_First, _Last);
}

template <class _BidIt>
inline void _Reverse_unchecked1(const _BidIt _First, const _BidIt _Last,
                                integral_constant<size_t, 2>) {
  // reverse elements in [_First, _Last), pointers to trivially swappable of
  // size 2
  __std_reverse_trivially_swappable_2(_First, _Last);
}

template <class _BidIt>
inline void _Reverse_unchecked1(const _BidIt _First, const _BidIt _Last,
                                integral_constant<size_t, 4>) {
  // reverse elements in [_First, _Last), pointers to trivially swappable of
  // size 4
  __std_reverse_trivially_swappable_4(_First, _Last);
}

template <class _BidIt>
inline void _Reverse_unchecked1(const _BidIt _First, const _BidIt _Last,
                                integral_constant<size_t, 8>) {
  // reverse elements in [_First, _Last), pointers to trivially swappable of
  // size 8
  __std_reverse_trivially_swappable_8(_First, _Last);
}
#endif  // (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_CEE_PURE) &&
        // !defined(_M_HYBRID)

template <class _BidIt>
inline void _Reverse_unchecked(const _BidIt _First, const _BidIt _Last) {
  // reverse elements in [_First, _Last), choose optimization
#if (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_CEE_PURE) && \
    !defined(_M_HYBRID)
  using _Elem = remove_pointer_t<_BidIt>;
  constexpr size_t _Opt = is_pointer_v<_BidIt>                         //
                                  && _Is_trivially_swappable_v<_Elem>  //
                                  && !is_volatile_v<_Elem>             //
                                  &&
                                  (sizeof(_Elem) == 1 || sizeof(_Elem) == 2 ||
                                   sizeof(_Elem) == 4 || sizeof(_Elem) == 8)
                              ? sizeof(_Elem)
                              : 0;
#else   // ^^^ vectorize ^^^ // vvv no vectorize vvv
  constexpr size_t _Opt = 0;
#endif  // (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_CEE_PURE) &&
        // !defined(_M_HYBRID)
  _Reverse_unchecked1(_First, _Last, integral_constant<size_t, _Opt>{});
}

template <class _BidIt>
inline void reverse(const _BidIt _First, const _BidIt _Last) {
  // reverse elements in [_First, _Last)
  _Adl_verify_range(_First, _Last);
  _Reverse_unchecked(_Get_unwrapped(_First), _Get_unwrapped(_Last));
}
#endif  // _HAS_IF_CONSTEXPR

#if _HAS_CXX17
template <class _ExPo, class _BidIt, _Enable_if_execution_policy_t<_ExPo> = 0>
inline void reverse(_ExPo&&, _BidIt _First, _BidIt _Last) noexcept {
  // reverse elements in [_First, _Last)
  // not parallelized as benchmarks show it isn't worth it
  return _STD reverse(_First, _Last);
}
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE rotate
template <class _BidIt>
inline pair<_BidIt, _BidIt> _Reverse_until_sentinel_unchecked(_BidIt _First,
                                                              _BidIt _Sentinel,
                                                              _BidIt _Last) {
  // reverse until either _First or _Last hits _Sentinel
  while (_First != _Sentinel && _Last != _Sentinel) {
    _STD iter_swap(_First, --_Last);
    ++_First;
  }

  return pair<_BidIt, _BidIt>(_First, _Last);
}

#if _HAS_IF_CONSTEXPR
template <class _FwdIt>
inline _FwdIt rotate(_FwdIt _First, _FwdIt _Mid, _FwdIt _Last) {
  // exchange the ranges [_First, _Mid) and [_Mid, _Last)
  // that is, rotates [_First, _Last) left by distance(_First, _Mid) positions
  // returns the iterator pointing at *_First's new home
  _Adl_verify_range(_First, _Mid);
  _Adl_verify_range(_Mid, _Last);
  auto _UFirst = _Get_unwrapped(_First);
  auto _UMid = _Get_unwrapped(_Mid);
  const auto _ULast = _Get_unwrapped(_Last);
  if (_UFirst == _UMid) {
    return _Last;
  }

  if (_UMid == _ULast) {
    return _First;
  }

  if constexpr (_Is_random_iter_v<_FwdIt>) {
    _STD reverse(_UFirst, _UMid);
    _STD reverse(_UMid, _ULast);
    _STD reverse(_UFirst, _ULast);
    _Seek_wrapped(_First, _UFirst + (_ULast - _UMid));
  } else if constexpr (_Is_bidi_iter_v<_FwdIt>) {
    _STD reverse(_UFirst, _UMid);
    _STD reverse(_UMid, _ULast);
    auto _Tmp = _Reverse_until_sentinel_unchecked(_UFirst, _UMid, _ULast);
    _STD reverse(_Tmp.first, _Tmp.second);
    _Seek_wrapped(_First, _UMid != _Tmp.first ? _Tmp.first : _Tmp.second);
  } else {
    auto _UNext = _UMid;
    do {  // rotate the first cycle
      _STD iter_swap(_UFirst, _UNext);
      ++_UFirst;
      ++_UNext;
      if (_UFirst == _UMid) {
        _UMid = _UNext;
      }
    } while (_UNext != _ULast);
    _Seek_wrapped(_First, _UFirst);
    while (_UMid != _ULast) {  // rotate subsequent cycles
      _UNext = _UMid;
      do {
        _STD iter_swap(_UFirst, _UNext);
        ++_UFirst;
        ++_UNext;
        if (_UFirst == _UMid) {
          _UMid = _UNext;
        }
      } while (_UNext != _ULast);
    }
  }

  return _First;
}
#else   // ^^^ _HAS_IF_CONSTEXPR ^^^ // vvv !_HAS_IF_CONSTEXPR vvv
template <class _FwdIt>
inline _FwdIt _Rotate_unchecked1(_FwdIt _First, _FwdIt _Mid, _FwdIt _Last,
                                 forward_iterator_tag) {
  // rotate [_First, _Last) left by distance(_First, _Mid) positions, forward
  // iterators
  for (_FwdIt _Next = _Mid, _Res = _Last;;) {  // swap [_First, ...) into place
    _STD iter_swap(_First, _Next);
    if (++_First == _Mid) {  // quit if done, else define next interval
      if (++_Next == _Last) {
        return _Res == _Last ? _Mid : _Res;
      } else {
        _Mid = _Next;  // mark end of next interval
      }
    } else if (++_Next == _Last) {  // wrap to last end
      if (_Res == _Last) {
        _Res = _First;
      }

      _Next = _Mid;
    }
  }
}

template <class _BidIt>
inline _BidIt _Rotate_unchecked1(_BidIt _First, _BidIt _Mid, _BidIt _Last,
                                 bidirectional_iterator_tag) {
  // rotate [_First, _Last) left by distance(_First, _Mid) positions,
  // bidirectional iterators
  _Reverse_unchecked(_First, _Mid);
  _Reverse_unchecked(_Mid, _Last);
  auto _Tmp = _Reverse_until_sentinel_unchecked(_First, _Mid, _Last);
  _Reverse_unchecked(_Tmp.first, _Tmp.second);
  return _Mid != _Tmp.first ? _Tmp.first : _Tmp.second;
}

template <class _RanIt>
inline _RanIt _Rotate_unchecked1(_RanIt _First, _RanIt _Mid, _RanIt _Last,
                                 random_access_iterator_tag) {
  // rotate [_First, _Last) left by distance(_First, _Mid) positions,
  // random-access iterators
  _Reverse_unchecked(_First, _Mid);
  _Reverse_unchecked(_Mid, _Last);
  _Reverse_unchecked(_First, _Last);
  return _First + (_Last - _Mid);
}

template <class _FwdIt>
inline _FwdIt _Rotate_unchecked(_FwdIt _First, _FwdIt _Mid, _FwdIt _Last) {
  // rotate [_First, _Last) left by distance(_First, _Mid) positions
  if (_First == _Mid) {
    return _Last;
  }

  if (_Mid == _Last) {
    return _First;
  }

  return _Rotate_unchecked1(_First, _Mid, _Last, _Iter_cat_t<_FwdIt>());
}

template <class _FwdIt>
inline _FwdIt rotate(_FwdIt _First, _FwdIt _Mid, _FwdIt _Last) {
  // exchange the ranges [_First, _Mid) and [_Mid, _Last)
  // that is, rotates [_First, _Last) left by distance(_First, _Mid) positions
  // returns the iterator pointing at *_First's new home
  _Adl_verify_range(_First, _Mid);
  _Adl_verify_range(_Mid, _Last);
  _Seek_wrapped(_First,
                _Rotate_unchecked(_Get_unwrapped(_First), _Get_unwrapped(_Mid),
                                  _Get_unwrapped(_Last)));
  return _First;
}
#endif  // _HAS_IF_CONSTEXPR

#if _HAS_CXX17
template <class _ExPo, class _FwdIt, _Enable_if_execution_policy_t<_ExPo> = 0>
inline _FwdIt rotate(_ExPo&&, _FwdIt _First, _FwdIt _Mid,
                     _FwdIt _Last) noexcept {
  // rotate [_First, _Last) left by distance(_First, _Mid) positions
  // not parallelized as benchmarks show it isn't worth it
  return _STD rotate(_First, _Mid, _Last);
}
#endif  // _HAS_CXX17

// FUNCTION TEMPLATE lower_bound
template <class _FwdIt, class _Ty, class _Pr>
_NODISCARD inline _FwdIt lower_bound(_FwdIt _First, const _FwdIt _Last,
                                     const _Ty& _Val, _Pr _Pred) {
  // find first element not before _Val, using _Pred
  _Adl_verify_range(_First, _Last);
  auto _UFirst = _Get_unwrapped(_First);
  _Iter_diff_t<_FwdIt> _Count = _STD distance(_UFirst, _Get_unwrapped(_Last));

  while (0 < _Count) {  // divide and conquer, find half that contains answer
    const _Iter_diff_t<_FwdIt> _Count2 = _Count >> 1;  // TRANSITION, VSO#433486
    const auto _UMid = _STD next(_UFirst, _Count2);
    if (_Pred(*_UMid, _Val)) {  // try top half
      _UFirst = _Next_iter(_UMid);
      _Count -= _Count2 + 1;
    } else {
      _Count = _Count2;
    }
  }

  _Seek_wrapped(_First, _UFirst);
  return _First;
}

// CLASS TEMPLATE _Rng_from_urng
template <class _Diff, class _Urng>
class _Rng_from_urng {  // wrap a URNG as an RNG
 public:
  using _Ty0 = make_unsigned_t<_Diff>;
  using _Ty1 = typename _Urng::result_type;

  using _Udiff = conditional_t<sizeof(_Ty1) < sizeof(_Ty0), _Ty0, _Ty1>;

  explicit _Rng_from_urng(_Urng& _Func)
      : _Ref(_Func), _Bits(CHAR_BIT * sizeof(_Udiff)), _Bmask(_Udiff(-1)) {
    for (; (_Urng::max)() - (_Urng::min)() < _Bmask; _Bmask >>= 1) {
      --_Bits;
    }
  }

  _Diff operator()(_Diff _Index) {  // adapt _Urng closed range to [0, _Index)
    for (;;) {                      // try a sample random value
      _Udiff _Ret = 0;              // random bits
      _Udiff _Mask = 0;             // 2^N - 1, _Ret is within [0, _Mask]

      while (_Mask < _Udiff(_Index - 1)) {  // need more random bits
        _Ret <<= _Bits - 1;                 // avoid full shift
        _Ret <<= 1;
        _Ret |= _Get_bits();
        _Mask <<= _Bits - 1;  // avoid full shift
        _Mask <<= 1;
        _Mask |= _Bmask;
      }

      // _Ret is [0, _Mask], _Index - 1 <= _Mask, return if unbiased
      if (_Ret / _Index < _Mask / _Index ||
          _Mask % _Index == _Udiff(_Index - 1)) {
        return static_cast<_Diff>(_Ret % _Index);
      }
    }
  }

  _Udiff _Get_all_bits() {  // return a random value
    _Udiff _Ret = 0;

    for (size_t _Num = 0; _Num < CHAR_BIT * sizeof(_Udiff);
         _Num += _Bits) {  // don't mask away any bits
      _Ret <<= _Bits - 1;  // avoid full shift
      _Ret <<= 1;
      _Ret |= _Get_bits();
    }

    return _Ret;
  }

  _Rng_from_urng(const _Rng_from_urng&) = delete;
  _Rng_from_urng& operator=(const _Rng_from_urng&) = delete;

 private:
  _Udiff _Get_bits() {  // return a random value within [0, _Bmask]
    for (;;) {          // repeat until random value is in range
      _Udiff _Val = _Ref() - (_Urng::min)();

      if (_Val <= _Bmask) {
        return _Val;
      }
    }
  }

  _Urng& _Ref;    // reference to URNG
  size_t _Bits;   // number of random bits generated by _Get_bits()
  _Udiff _Bmask;  // 2^_Bits - 1
};

// CLASS TEMPLATE _Yarn
template <class _Elem>
class _CRTIMP2_PURE_IMPORT _Yarn {  // wrap a NTBS
 public:
  __CLR_OR_THIS_CALL _Yarn() : _Myptr(nullptr), _Nul(0) {}

  __CLR_OR_THIS_CALL _Yarn(const _Yarn& _Right) : _Myptr(nullptr), _Nul(0) {
    *this = _Right;
  }

  __CLR_OR_THIS_CALL _Yarn(const _Elem* _Right) : _Myptr(nullptr), _Nul(0) {
    *this = _Right;
  }

  _Yarn& __CLR_OR_THIS_CALL operator=(const _Yarn& _Right) {
    return *this = _Right._Myptr;
  }

  _Yarn& __CLR_OR_THIS_CALL
  operator=(const _Elem* _Right) {  // assign from NTBS
    if (_Myptr != _Right) {         // new value, discard old and copy new
      _Tidy();

      if (_Right != nullptr) {  // new is not empty, copy it
        const _Elem* _Ptr = _Right;
        while (*_Ptr != (_Elem)0) {
          ++_Ptr;
        }

        const auto _Count = (++_Ptr - _Right) * sizeof(_Elem);

#ifdef _DEBUG
        _Myptr = (_Elem*)_malloc_dbg(_Count, _CRT_BLOCK, __FILE__, __LINE__);
#else   // _DEBUG
        _Myptr = (_Elem*)_CSTD malloc(_Count);
#endif  // _DEBUG

        if (_Myptr != nullptr) {
          _CSTD memcpy(_Myptr, _Right, _Count);
        }
      }
    }

    return *this;
  }

  __CLR_OR_THIS_CALL ~_Yarn() noexcept { _Tidy(); }

  bool __CLR_OR_THIS_CALL empty() const { return _Myptr == nullptr; }

  _Ret_z_ const _Elem* __CLR_OR_THIS_CALL c_str() const {
    return _Myptr != nullptr ? _Myptr : &_Nul;
  }

  bool __CLR_OR_THIS_CALL _Empty() const { return _Myptr == nullptr; }

  _Ret_z_ const _Elem* __CLR_OR_THIS_CALL _C_str() const {
    return _Myptr != nullptr ? _Myptr : &_Nul;
  }

 private:
  void __CLR_OR_THIS_CALL _Tidy() noexcept {
    if (_Myptr != nullptr) {
#ifdef _DEBUG
      _free_dbg(_Myptr, _CRT_BLOCK);
#else   // _DEBUG
      _CSTD free(_Myptr);
#endif  // _DEBUG
    }

    _Myptr = nullptr;
  }

  _Elem* _Myptr;  // pointer to allocated string
  _Elem _Nul;     // nul terminator for unallocated string
};

// CLASS TEMPLATE back_insert_iterator
template <class _Container>
class back_insert_iterator {  // wrap pushes to back of container as output
                              // iterator
 public:
  using iterator_category = output_iterator_tag;
  using value_type = void;
  using difference_type = void;
  using pointer = void;
  using reference = void;

  using container_type = _Container;

  explicit back_insert_iterator(_Container& _Cont)
      : container(_STD addressof(_Cont)) {  // construct with container
  }

  back_insert_iterator& operator=(const typename _Container::value_type&
                                      _Val) {  // push value into container
    container->push_back(_Val);
    return *this;
  }

  back_insert_iterator& operator=(
      typename _Container::value_type&& _Val) {  // push value into container
    container->push_back(_STD move(_Val));
    return *this;
  }

  _NODISCARD back_insert_iterator&
  operator*() {  // pretend to return designated value
    return *this;
  }

  back_insert_iterator& operator++() {  // pretend to preincrement
    return *this;
  }

  back_insert_iterator operator++(int) {  // pretend to postincrement
    return *this;
  }

 protected:
  _Container* container;  // pointer to container
};

// FUNCTION TEMPLATE back_inserter
template <class _Container>
_NODISCARD inline back_insert_iterator<_Container> back_inserter(
    _Container& _Cont) {  // return a back_insert_iterator
  return back_insert_iterator<_Container>(_Cont);
}

// STRUCT TEMPLATE _Has_allocator_type
template <class _Ty, class _Alloc,
          class = void>
struct _Has_allocator_type
    : false_type {  // tests for suitable _Ty::allocator_type
};

template <class _Ty, class _Alloc>
struct _Has_allocator_type<_Ty, _Alloc, void_t<typename _Ty::allocator_type>>
    : is_convertible<_Alloc, typename _Ty::allocator_type>::
          type {  // tests for suitable _Ty::allocator_type
};

// STRUCT allocator_arg_t
struct allocator_arg_t {  // tag type for added allocator argument
  explicit allocator_arg_t() = default;
};

_INLINE_VAR constexpr allocator_arg_t allocator_arg{};

[[noreturn]] _CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL _Xbad_alloc();
[[noreturn]] _CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL
_Xinvalid_argument(_In_z_ const char*);
[[noreturn]] _CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL
_Xlength_error(_In_z_ const char*);
[[noreturn]] _CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL
_Xout_of_range(_In_z_ const char*);
[[noreturn]] _CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL
_Xoverflow_error(_In_z_ const char*);
[[noreturn]] _CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL
_Xruntime_error(_In_z_ const char*);

// STRUCT TEMPLATE uses_allocator
template <class _Ty, class _Alloc>
struct uses_allocator
    : _Has_allocator_type<_Ty, _Alloc>::type {  // determine whether _Ty has an
                                                // allocator_type member type
};

template <class _Ty, class _Alloc>
_INLINE_VAR constexpr bool uses_allocator_v =
    uses_allocator<_Ty, _Alloc>::value;

// STRUCT TEMPLATE iterator
template <class _Category, class _Ty, class _Diff = ptrdiff_t,
          class _Pointer = _Ty*,
          class _Reference = _Ty&>
struct _CXX17_DEPRECATE_ITERATOR_BASE_CLASS
    iterator {  // base type for iterator classes
  using iterator_category = _Category;
  using value_type = _Ty;
  using difference_type = _Diff;
  using pointer = _Pointer;
  using reference = _Reference;
};

// STRUCT TEMPLATE _Tidy_guard
template <class _Ty>
struct _Tidy_guard {  // class with destructor that calls _Tidy
  _Ty* _Target;
  ~_Tidy_guard() {
    if (_Target) {
      _Target->_Tidy();
    }
  }
};

// STRUCT TEMPLATE _Tidy_deallocate_guard
template <class _Ty>
struct _Tidy_deallocate_guard {  // class with destructor that calls
                                 // _Tidy_deallocate
  _Ty* _Target;
  ~_Tidy_deallocate_guard() {
    if (_Target) {
      _Target->_Tidy_deallocate();
    }
  }
};
_STD_END
#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)
#endif  // RC_INVOKED
#endif  // _XUTILITY_

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */