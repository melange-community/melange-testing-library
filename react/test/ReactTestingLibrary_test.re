[@coverage exclude_file];

open Jest;

afterAll(Bisect_melange.Runtime.write_coverage_data);

module Greeting = {
  [@react.component]
  let make = (~message) => <div> {React.string(message)} </div>;
};

module Counter = {
  type action =
    | Inc
    | Dec;

  [@react.component]
  let make = () => {
    let (state, dispatch) =
      React.useReducer(
        (state, action) =>
          switch (action) {
          | Inc => state + 1
          | Dec => state - 1
          },
        0,
      );

    <div>
      {React.string("Count: " ++ string_of_int(state))}
      <button onClick={_event => dispatch(Inc)}>
        {React.string("+")}
      </button>
      <button onClick={_event => dispatch(Dec)}>
        {React.string("-")}
      </button>
    </div>;
  };
};

external unsafeAsElement: Dom.node => Dom.element = "%identity";
[@mel.get] external firstChild: Dom.element => Dom.node = "firstChild";
[@mel.get] external innerHTML: Dom.node => string = "innerHTML";

describe("ReactTestingLibrary", () => {
  open ReactTestingLibrary;
  open Expect;

  let element =
    <div style={ReactDOM.Style.make(~color="rebeccapurple", ())}>
      {React.cloneElement(
         <h1> {React.string("Heading")} </h1>,
         {"data-testid": "h1-heading"},
       )}
    </div>;

  test("render works", () =>
    element
    |> render
    |> container
    |> firstChild
    |> unsafeAsElement
    |> JestDom.expect
    |> JestDom.toBeInTheDocument
  );

  // ByLabelText
  describe("ByLabelText", () => {
    let labelText =
      <div>
        <label htmlFor="username-input"> {React.string("Username")} </label>
        <input id="username-input" />
      </div>;

    test("getByLabelText works", () =>
      labelText
      |> render
      |> getByLabelText(~matcher=`Str("Username"))
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("getAllByLabelText works", () =>
      labelText
      |> render
      |> getAllByLabelText(~matcher=`Str("Username"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByLabelText works", () =>
      labelText
      |> render
      |> queryByLabelText(~matcher=`Str("Username"))
      |> Js.Null.getExn
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByLabelText works (element not found)", () =>
      labelText
      |> render
      |> queryByLabelText(~matcher=`Str("!@#$Username!@#$"))
      |> expect
      |> toEqual(Js.null)
    );

    test("queryAllByLabelText works", () =>
      labelText
      |> render
      |> queryAllByLabelText(~matcher=`Str("Username"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    testPromise("findByLabelText works", () =>
      labelText
      |> render
      |> findByLabelText(~matcher=`Str("Username"))
      |> Js.Promise.then_(result =>
           result
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );

    testPromise("findAllByLabelText works", () =>
      labelText
      |> render
      |> findAllByLabelText(~matcher=`Str("Username"))
      |> Js.Promise.then_(many =>
           many
           |> Js.Array.unsafe_get(_, 0)
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );
  });

  // ByPlaceholderText
  describe("ByPlaceholderText", () => {
    let placeholderText =
      <div>
        <input placeholder="Username" />
        <input placeholder="Password" />
      </div>;

    test("getByPlaceholderText works", () =>
      placeholderText
      |> render
      |> getByPlaceholderText(~matcher=`Str("Username"))
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("getAllByPlaceholderText works", () =>
      placeholderText
      |> render
      |> getAllByPlaceholderText(~matcher=`Str("Username"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByPlaceholderText works", () =>
      placeholderText
      |> render
      |> queryByPlaceholderText(~matcher=`Str("Username"))
      |> Js.Null.getExn
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByPlaceholderText works (element not found)", () =>
      placeholderText
      |> render
      |> queryByPlaceholderText(~matcher=`Str("!@#$Username!@#$"))
      |> expect
      |> toEqual(Js.null)
    );

    test("queryAllByPlaceholderText works", () =>
      placeholderText
      |> render
      |> queryAllByPlaceholderText(~matcher=`Str("Username"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    testPromise("findByPlaceholderText works", () =>
      placeholderText
      |> render
      |> findByPlaceholderText(~matcher=`Str("Username"))
      |> Js.Promise.then_(result =>
           result
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );

    testPromise("findAllByPlaceholderText works", () =>
      placeholderText
      |> render
      |> findAllByPlaceholderText(~matcher=`Str("Username"))
      |> Js.Promise.then_(many =>
           many
           |> Js.Array.unsafe_get(_, 0)
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );
  });

  // ByText
  describe("ByText", () => {
    let text =
      <div> <a href="/about"> {React.string({j|About ℹ️|j})} </a> </div>;

    test("getByText works", () =>
      text
      |> render
      |> getByText(~matcher=`Str({j|About ℹ️|j}))
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("getAllByText works", () =>
      text
      |> render
      |> getAllByText(~matcher=`Str({j|About ℹ️|j}))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByText works", () =>
      text
      |> render
      |> queryByText(~matcher=`Str({j|About ℹ️|j}))
      |> Js.Null.getExn
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByText works (element not found)", () =>
      text
      |> render
      |> queryByText(~matcher=`Str({j|!@#About ℹ️!@#|j}))
      |> expect
      |> toEqual(Js.null)
    );

    test("queryAllByText works", () =>
      text
      |> render
      |> queryAllByText(~matcher=`Str({j|About ℹ️|j}))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    testPromise("findByText works", () =>
      text
      |> render
      |> findByText(~matcher=`Str({j|About ℹ️|j}))
      |> Js.Promise.then_(result =>
           result
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );

    testPromise("findAllByText works", () =>
      text
      |> render
      |> findAllByText(~matcher=`Str({j|About ℹ️|j}))
      |> Js.Promise.then_(many =>
           many
           |> Js.Array.unsafe_get(_, 0)
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );
  });

  // ByAltText
  describe("ByAltText", () => {
    let altText =
      <div>
        <img alt="Incredibles 1 Poster" src="/incredibles-1.png" />
        <img alt="Incredibles 2 Poster" src="/incredibles-2.png" />
      </div>;

    test("getByAltText works", () =>
      altText
      |> render
      |> getByAltText(~matcher=`Str("Incredibles 2 Poster"))
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("getAllByAltText works", () =>
      altText
      |> render
      |> getAllByAltText(~matcher=`Str("Incredibles 2 Poster"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByAltText works", () =>
      altText
      |> render
      |> queryByAltText(~matcher=`Str("Incredibles 2 Poster"))
      |> Js.Null.getExn
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByAltText works (element not found)", () =>
      altText
      |> render
      |> queryByAltText(~matcher=`Str("!@#$Incredibles 2 Poster!@#$"))
      |> expect
      |> toEqual(Js.null)
    );

    test("queryAllByAltText works", () =>
      altText
      |> render
      |> queryAllByAltText(~matcher=`Str("Incredibles 2 Poster"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    testPromise("findByAltText works", () =>
      altText
      |> render
      |> findByAltText(~matcher=`Str("Incredibles 2 Poster"))
      |> Js.Promise.then_(result =>
           result
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );

    testPromise("findAllByAltText works", () =>
      altText
      |> render
      |> findAllByAltText(~matcher=`Str("Incredibles 2 Poster"))
      |> Js.Promise.then_(many =>
           many
           |> Js.Array.unsafe_get(_, 0)
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );
  });

  // ByTitle
  describe("ByTitle", () => {
    let title =
      <div>
        <span title="Delete" id="2" />
        <svg>
          <title> {React.string("Close")} </title>
          <g> <path /> </g>
        </svg>
      </div>;

    test("getByTitle works", () =>
      title
      |> render
      |> getByTitle(~matcher=`Str("Delete"))
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("getAllByTitle works", () =>
      title
      |> render
      |> getAllByTitle(~matcher=`Str("Delete"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByTitle works", () =>
      title
      |> render
      |> queryByTitle(~matcher=`Str("Delete"))
      |> Js.Null.getExn
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByTitle works (element not found)", () =>
      title
      |> render
      |> queryByTitle(~matcher=`Str("!@#$Delete!@#$"))
      |> expect
      |> toEqual(Js.null)
    );

    test("queryAllByTitle works", () =>
      title
      |> render
      |> queryAllByTitle(~matcher=`Str("Delete"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    testPromise("findByTitle works", () =>
      title
      |> render
      |> findByTitle(~matcher=`Str("Delete"))
      |> Js.Promise.then_(result =>
           result
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );

    testPromise("findAllByTitle works", () =>
      title
      |> render
      |> findAllByTitle(~matcher=`Str("Delete"))
      |> Js.Promise.then_(many =>
           many
           |> Js.Array.unsafe_get(_, 0)
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );
  });

  // ByDisplayValue
  describe("ByDisplayValue", () => {
    let title =
      <div>
        <input type_="text" id="lastName" defaultValue="ReasonML" />
      </div>;

    test("getByDisplayValue works", () =>
      title
      |> render
      |> getByDisplayValue(~matcher=`Str("ReasonML"))
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("getAllByDisplayValue works", () =>
      title
      |> render
      |> getAllByDisplayValue(~matcher=`Str("ReasonML"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByDisplayValue works", () =>
      title
      |> render
      |> queryByDisplayValue(~matcher=`Str("ReasonML"))
      |> Js.Null.getExn
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByDisplayValue works (element not found)", () =>
      title
      |> render
      |> queryByDisplayValue(~matcher=`Str("!@#$ReasonML!@#$"))
      |> expect
      |> toEqual(Js.null)
    );

    test("queryAllByDisplayValue works", () =>
      title
      |> render
      |> queryAllByDisplayValue(~matcher=`Str("ReasonML"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    testPromise("findByDisplayValue works", () =>
      title
      |> render
      |> findByDisplayValue(~matcher=`Str("ReasonML"))
      |> Js.Promise.then_(result =>
           result
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );

    testPromise("findAllByDisplayValue works", () =>
      title
      |> render
      |> findAllByDisplayValue(~matcher=`Str("ReasonML"))
      |> Js.Promise.then_(many =>
           many
           |> Js.Array.unsafe_get(_, 0)
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );
  });

  // ByRole
  describe("ByRole", () => {
    let role =
      <div role="dialog">
        <img ariaLabel="fancy image" src="fancy.jpg" />
        <button> {React.string("Close dialog")} </button>
      </div>;

    test("getByRole works", () =>
      role
      |> render
      |> getByRole(~matcher=`Str("button"))
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("getAllByRole works", () =>
      role
      |> render
      |> getAllByRole(~matcher=`Str("button"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByRole works", () =>
      role
      |> render
      |> queryByRole(~matcher=`Str("button"))
      |> Js.Null.getExn
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByRole works (element not found)", () =>
      role
      |> render
      |> queryByRole(~matcher=`Str("!@#$button!@#$"))
      |> expect
      |> toEqual(Js.null)
    );

    test("queryAllByRole works", () =>
      role
      |> render
      |> queryAllByRole(~matcher=`Str("button"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    testPromise("findByRole works", () =>
      role
      |> render
      |> findByRole(~matcher=`Str("button"))
      |> Js.Promise.then_(result =>
           result
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );

    testPromise("findAllByRole works", () =>
      role
      |> render
      |> findAllByRole(~matcher=`Str("button"))
      |> Js.Promise.then_(many =>
           many
           |> Js.Array.unsafe_get(_, 0)
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );
  });

  // ByTestId
  describe("ByTestId", () => {
    test("getByTestId works", () =>
      element
      |> render
      |> getByTestId(~matcher=`Str("h1-heading"))
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("getAllByTestId works", () =>
      element
      |> render
      |> getAllByTestId(~matcher=`Str("h1-heading"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByTestId works", () =>
      element
      |> render
      |> queryByTestId(~matcher=`Str("h1-heading"))
      |> Js.Null.getExn
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    test("queryByTestId works (element not found)", () =>
      element
      |> render
      |> queryByTestId(~matcher=`Str("!@#$h1-heading!@#$"))
      |> expect
      |> toEqual(Js.null)
    );

    test("queryAllByTestId works", () =>
      element
      |> render
      |> queryAllByTestId(~matcher=`Str("h1-heading"))
      |> Js.Array.unsafe_get(_, 0)
      |> JestDom.expect
      |> JestDom.toBeInTheDocument
    );

    testPromise("findByTestId works", () =>
      element
      |> render
      |> findByTestId(~matcher=`Str("h1-heading"))
      |> Js.Promise.then_(result =>
           result
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );

    testPromise("findAllByTestId works", () =>
      element
      |> render
      |> findAllByTestId(~matcher=`Str("h1-heading"))
      |> Js.Promise.then_(many =>
           many
           |> Js.Array.unsafe_get(_, 0)
           |> JestDom.expect
           |> JestDom.toBeInTheDocument
           |> Js.Promise.resolve
         )
    );
  });

  describe("debug", () => {
    beforeEach(() => {
      let _ = [%raw
        {|jest.spyOn(console, 'log').mockImplementation(() => {})|}
      ];
      ();
    });

    afterEach(() => {
      let _ = [%raw {|console.log.mockRestore()|}];
      ();
    });

    test("works", () => {
      let _ = element |> render |> debug();

      let _ = [%raw {|expect(console.log).toHaveBeenCalledTimes(1)|}];
      let _ = [%raw
        {|
        expect(console.log).toHaveBeenCalledWith(
          expect.stringContaining('Heading')
        )
      |}
      ];

      pass;
    });

    test("works with element argument", () => {
      let result = element |> render;
      let el = result |> container |> firstChild |> unsafeAsElement;

      let _ = result |> debug(~el, ());

      let _ = [%raw {|expect(console.log).toHaveBeenCalledTimes(1)|}];
      let _ = [%raw
        {|
        expect(console.log).toHaveBeenCalledWith(
          expect.stringContaining('Heading')
        )
      |}
      ];

      pass;
    });

    test("works with maxLengthToPrint argument", () => {
      let result = element |> render;
      let el = result |> container |> firstChild |> unsafeAsElement;

      let _ = result |> debug(~el, ~maxLengthToPrint=25, ());

      let _ = [%raw {|expect(console.log).toHaveBeenCalledTimes(1)|}];
      let _ = [%raw
        {|
        expect(console.log).toHaveBeenCalledWith(
          expect.stringContaining('sty...')
        )
      |}
      ];

      pass;
    });
  });

  test("rerender works", () => {
    let result = render(<Greeting message="hi" />);
    let check = text =>
      result
      |> container
      |> firstChild
      |> innerHTML
      |> expect
      |> toEqual(text);

    check("hi") |> ignore;

    result |> rerender(<Greeting message="hey" />);

    check("hey");
  });

  test("asFragment works", () => {
    let fragment = element |> render |> asFragment();
    let textContent = Webapi.Dom.DocumentFragment.textContent(fragment);
    expect(textContent) |> toEqual("Heading");
  });

  test("act works", () => {
    let result = <Counter /> |> render;

    act(() => result |> getByText(~matcher=`Str("+")) |> FireEvent.click);

    result
    |> getByText(~matcher=`Str("Count: 1"))
    |> JestDom.expect
    |> JestDom.toBeInTheDocument;
  });

  testPromise("Cleaunp, (element not found)", () => {
    let result = element |> render;

    cleanup();

    Js.Promise.resolve(
      result
      |> queryByTestId(~matcher=`Str("h1-heading"))
      |> expect
      |> toEqual(Js.null),
    );
  });
});
