namespace hermit {
  template<
    typename ...Rule
  >
  struct priority {};
  template<
    typename Rule
  >
  struct priority : public Rule {};
  template<
    typename ...Rules,
    typename Rule
  >
  struct priority : public boost::mpl::or< Rule, priority< Rules... > > {};
}

