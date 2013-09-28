.. index::
    single: Forms
    single: Components; Form

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/10/02

Formコンポーネント
==================

    Formコンポーネントを使うと、再利用可能なHTMLフォームを簡単に作ることができます。

フォームはエンドユーザからのデータ入力を受け付け、アプリケーションのデータを変更するための機能です。
一般的にフォームというと、HTMLフォームを指すことが多いですが、Formコンポーネントではクライアントとアプリケーション間で送受信されたデータを、送信形態に関わらず処理することに焦点を当てています。

インストール
------------

Formコンポーネントは以下の方法でインストールができます。

* 公式Gitリポジトリ (https://github.com/symfony/Form);
* Composerを使ってインストール (\ `Packagist` の ``symfony/form``\ ).

初期設定
--------

.. tip::

    Symfony Standard Editionを使用していれば、既にFormコンポーネントを使うための基本的な設定がされています。
    このセクションをスキップする場合は :ref:`component-form-intro-create-simple-form` に進んで下さい。

Symfony2ではフォームは\ *フォームファクトリ*\ によって構築されるオブジェクトでできています。フォームファクトリの生成はとてもシンプルです::

    use Symfony¥Component¥Form¥Forms;

    $formFactory = Forms::createFormFactory();

このファクトリでも基本的なフォームを作ることができますが、以下に挙げる重要な機能をサポートしていません。

* **Requestハンドリング:** リクエストの処理とファイルアップロードの処理をサポート;
* **CSRFプロテクション:** クロスサイトリクエストフォージェリ攻撃への対策;
* **テンプレーティング:** フォームのレンダリングに使用するHTMLの断片化と再利用を可能にするテンプレーティングレイヤを統合;
* **翻訳:** エラーメッセージやフィールドラベルなどの多言語化の対応;
* **バリデーション:** 送信されたデータを検証し、エラーメッセージを生成するバリデーションライブラリを統合;

Formコンポーネントでこれらの機能を使うには他のライブラリとの連携が必要となります。
最も利用されるライブラリはTwigおよびSymfonyの :doc:`HttpFoundation</components/http_foundation/introduction>`\ 、
Translation、Validatorコンポーネントですが、これらは自分で選んだ他のライブラリに置き換えることも可能です。

次のセクションでは、これらのライブラリをフォームファクトリに組み込む手順を説明します。

.. tip::

    実装例は https://github.com/bschussek/standalone-forms で見ることができます。

Requestハンドリング
~~~~~~~~~~~~~~~~~~~

フォームのデータを処理するには、リクエストされた情報（通常は ``$_POST``\ ）を補足し、
:method:`Symfony¥¥Component¥¥Form¥¥Form::bind` メソッドに送信された配列データを渡す必要があります。
Formコンポーネントはこれをより簡単にするためにSymfonyの :doc:`HttpFoundation</components/http_foundation/introduction>` コンポーネントとの連携が可能です。

HttpFoundationコンポーネントと連携するにはフォームファクトリに :class:`Symfony¥¥Component¥¥Form¥¥Extension¥¥HttpFoundation¥¥HttpFoundationExtension` を追加する必要があります::

    use Symfony¥Component¥Form¥Forms;
    use Symfony¥Component¥Form¥Extension¥HttpFoundation¥HttpFoundationExtension;

    $formFactory = Forms::createFormFactoryBuilder()
        ->addExtension(new HttpFoundationExtension())
        ->getFormFactory();

これでデータを処理する際、配列の代わりに :class:`Symfony¥¥Component¥¥HttpFoundation¥¥Request` オブジェクトを
:method:`Symfony¥¥Component¥¥Form¥¥Form::bind` に渡すことができるようになります。

.. note::

    ``HttpFoundation`` コンポーネントの詳細、インストール方法などについては :doc:`/components/http_foundation/introduction` をご覧ください。

CSRFプロテクション
~~~~~~~~~~~~~~~~~~

CSRF攻撃への対策はFormコンポーネントに組み込まれていますが、明示的に有効にするか、
独自の方法に置き換える必要があります。以下のスニペットではCSRFエクステンションをフォームファクトリに追加しています::

    use Symfony¥Component¥Form¥Forms;
    use Symfony¥Component¥Form¥Extension¥Csrf¥CsrfExtension;
    use Symfony¥Component¥Form¥Extension¥Csrf¥CsrfProvider¥SessionCsrfProvider;
    use Symfony¥Component¥HttpFoundation¥Session¥Session;

    // generate a CSRF secret from somewhere
    $csrfSecret = '<generated token>';

    // create a Session object from the HttpFoundation component
    $session = new Session();

    $csrfProvider = new SessionCsrfProvider($session, $csrfSecret);

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->addExtension(new CsrfExtension($csrfProvider))
        ->getFormFactory();

CSRF攻撃からアプリケーションを保護するには、CSRFシークレットを定義する必要があります。
少なくとも32文字のランダムな文字列を生成し、上記のコードに追加して、Webサーバ以外はシークレットを参照できないことを確認して下さい。

内部では、このエクステンションはすべてのフォームに値が自動生成された状態のhiddenフィールド（デフォルト名は ``__token``\ ）を自動で追加し、データをバインドする際に検証されます。

.. tip::

    HttpFoundationコンポーネントを使用していない場合は、:class:`Symfony¥¥Component¥¥Form¥¥Extension¥¥Csrf¥¥CsrfProvider¥¥DefaultCsrfProvider` を代わりに使用して下さい。
    これはPHPのネイティブのセッションを使います::

        use Symfony¥Component¥Form¥Extension¥Csrf¥CsrfProvider¥DefaultCsrfProvider;

        $csrfProvider = new DefaultCsrfProvider($csrfSecret);

Twigテンプレーティング
~~~~~~~~~~~~~~~~~~~~~~

HTMLフォームを処理するためにFormコンポーネントを使用している場合、簡単にフォームを（フィールド値、エラーおよびラベルを完備した）HTMLフォームのフィールドとしてレンダリングする方法が必要になります。
テンプレートエンジンに `Twig`_ を使用していれば、リッチなインテグレーションを使うことができます。

このインテグレーションの利用には ``TwigBridge`` が必要になります。TwigBridgeはTwigといくつかのSymfonyコンポーネントを取りまとめます。
Composerを使用している場合、``composer.json`` の ``require`` に次の行を追加すると最新のバージョン2.3をインストールすることができます。

.. code-block:: json

    {
        "require": {
            "symfony/twig-bridge": "2.3.*"
        }
    }

TwigBridgeを使うと、各フィールド（およびそれに付随する物）のためのHTMLウィジェット、ラベル、フィールドのエラーのレンダリングに役立つ様々な
:doc:`Twig Functions</reference/forms/twig_reference>` が有効になります。連携にはTwigのインスタンスを初期化または取得し、
:class:`Symfony¥¥Bridge¥¥Twig¥¥Extension¥¥FormExtension` を追加する必要があります::

    use Symfony¥Component¥Form¥Forms;
    use Symfony¥Bridge¥Twig¥Extension¥FormExtension;
    use Symfony¥Bridge¥Twig¥Form¥TwigRenderer;
    use Symfony¥Bridge¥Twig¥Form¥TwigRendererEngine;

    // the Twig file that holds all the default markup for rendering forms
    // this file comes with TwigBridge
    $defaultFormTheme = 'form_div_layout.html.twig';

    $vendorDir = realpath(__DIR__ . '/../vendor');
    // the path to TwigBridge so Twig can locate the form_div_layout.html.twig file
    $vendorTwigBridgeDir = $vendorDir . '/symfony/twig-bridge/Symfony/Bridge/Twig';
    // the path to your other templates
    $viewsDir = realpath(__DIR__ . '/../views');

    $twig = new Twig_Environment(new Twig_Loader_Filesystem(array(
        $viewsDir,
        $vendorTwigBridgeDir . '/Resources/views/Form',
    )));
    $formEngine = new TwigRendererEngine(array($defaultFormTheme));
    $formEngine->setEnvironment($twig);
    // add the FormExtension to Twig
    $twig->addExtension(new FormExtension(new TwigRenderer($formEngine, $csrfProvider)));

    // create your form factory as normal
    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->getFormFactory();

厳密には `Twig Configuration`_ の詳細は異なるでしょうが、\ :class:`Symfony¥¥Bridge¥¥Twig¥¥Extension¥¥FormExtension`
をTwigに追加するという点は共通です。そのためにはまず、あなたの :ref:`form themes<cookbook-form-customization-form-themes>`
（つまり、フォームのHTMLマークアップを定義する resources/files）を定義する :class:`Symfony¥¥Bridge¥¥Twig¥¥Form¥¥TwigRendererEngine` を作る必要があります。

一般的なフォームのレンダリングについては :doc:`/cookbook/form/form_customization` を参照のこと。

.. note::

    Twigを用いた翻訳フィルタの使い方については下記の「:ref:`component-form-intro-install-translation`」をお読み下さい。

.. _component-form-intro-install-translation:

翻訳
~~~~

Twigのデフォルトのフォームテーマのいずれかを (例えば\ ``form_div_layout.html.twig``\ ) 使用していれば、
フォームラベルやエラーメッセージ、オプションテキストなどの翻訳に2つのTwigフィルタが利用できます。（\ ``trans``\ 、\ ``transChoice``\ ）

これらのTwigフィルタを使うには、Symfonyの ``Translation`` コンポーネントとの連携を実現した :class:`Symfony¥¥Bridge¥¥Twig¥¥Extension¥¥TranslationExtension`
を使用するか、自作のTwigエクステンションから2つのTwigフィルタを自分で追加することです。

``TranslationExtension`` を使用するには、プロジェクトにSymfonyの ``Translation`` コンポーネントと :doc:`Config</components/config/introduction>`
コンポーネントがインストールされていることを確認してください。Composerを使用してれば、\ ``composer.json`` に以下を追加することで最新のバージョン2.3をインストールすることができます。:

.. code-block:: json

    {
        "require": {
            "symfony/translation": "2.3.*",
            "symfony/config": "2.3.*"
        }
    }

次に、\ :class:`Symfony¥¥Bridge¥¥Twig¥¥Extension¥¥TranslationExtension` を ``Twig_Environment`` インスタンスに追加します::

    use Symfony¥Component¥Form¥Forms;
    use Symfony¥Component¥Translation¥Translator;
    use Symfony¥Component¥Translation¥Loader¥XliffFileLoader;
    use Symfony¥Bridge¥Twig¥Extension¥TranslationExtension;

    // create the Translator
    $translator = new Translator('en');
    // somehow load some translations into it
    $translator->addLoader('xlf', new XliffFileLoader());
    $translator->addResource(
        'xlf',
        __DIR__.'/path/to/translations/messages.en.xlf',
        'en'
    );

    // add the TranslationExtension (gives us trans and transChoice filters)
    $twig->addExtension(new TranslationExtension($translator));

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->getFormFactory();

これで、上記の翻訳ソースにフィールドラベルなどの文字列のキーと対応する翻訳を追加することができるようになりました。

翻訳についてさらに詳しい詳細は :doc:`/book/translation` をご覧ください。

バリデーション
~~~~~~~~~~~~~~

FormコンポーネントはSymfonyのValidatorコンポーネントと連携して検証を行うことができますが、既にある独自の検証システムを活用することもできます。
単にフォームにバインドされたデータ（配列またはオブジェクト）を取り、独自の検証システムに通すだけです。

SymfonyのValidatorコンポーネントを使用する場合は、まずプロジェクトにValidatorコンポーネントがインストールされている事を確認してください。
Composerを使用していれば、\ ``composer.json`` の ``require`` に次の行を追加すると最新のバージョン2.3をインストールすることができます。:

.. code-block:: json

    {
        "require": {
            "symfony/validator": "2.3.*"
        }
    }

Validatorコンポーネントに慣れていなければ :doc:`/book/validation` を参照して下さい。Formコンポーネントは
:class:`Symfony¥¥Component¥¥Form¥¥Extension¥¥Validator¥¥ValidatorExtension` を使用して自動的にバインドされたデータを検証し、エラーがあれば正しいフィールドにマップされた上でレンダリングします。

Validatorコンポーネントとの連携は次のようにして行います::

    use Symfony¥Component¥Form¥Forms;
    use Symfony¥Component¥Form¥Extension¥Validator¥ValidatorExtension;
    use Symfony¥Component¥Validator¥Validation;

    $vendorDir = realpath(__DIR__ . '/../vendor');
    $vendorFormDir = $vendorDir . '/symfony/form/Symfony/Component/Form';
    $vendorValidatorDir = $vendorDir . '/symfony/validator/Symfony/Component/Validator';

    // create the validator - details will vary
    $validator = Validation::createValidator();

    // there are built-in translations for the core error messages
    $translator->addResource(
        'xlf',
        $vendorFormDir . '/Resources/translations/validators.en.xlf',
        'en',
        'validators'
    );
    $translator->addResource(
        'xlf',
        $vendorValidatorDir . '/Resources/translations/validators.en.xlf',
        'en',
        'validators'
    );

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->addExtension(new ValidatorExtension($validator))
        ->getFormFactory();

更に詳しい情報は :ref:`component-form-intro-validation` セクションに記載されています。

フォームファクトリへのアクセス
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

フォームファクトリはアプリケーション中で1つだけ保持し、すべてのフォームはそこから構築されるべきです。
つまり、フォームの生成に必要な時にいつでもアクセスできるよう、フォームファクトリはアプリケーションのブートストラップ部分で生成しなければならないことを意味します。

.. note::

    このドキュメントではフォームファクトリをローカル変数の ``$formFactory`` に代入していますが、実際にはどこからでもアクセスできる「グローバル」な場所にインスタンスを保持する必要があります。

:term`Service Container` を使用していればフォームファクトリをそこに登録すると良いでしょう。
古典的であり非推奨ですがグローバル、またはスタティックな変数に格納するといった方法もあります。

アプリケーションの設計に関わらず、アプリケーション中でフォームファクトリは1つだけ保持し、どこからでもアクセスできるようにする必要があると言うことを覚えておいて下さい。

.. _component-form-intro-create-simple-form:

シンプルなフォームの作成
------------------------

.. tip::

    Symfony2フレームワークを使用していれば、フォームファクトリはサービス名 ``form.factory`` として登録されています。
    また、デフォルトのコントローラクラスは :method:`Symfony¥¥Bundle¥¥FrameworkBundle¥¥Controller::createFormBuilder` メソッドを持っています。
    これはフォームファクトリを取得し、``createBuilder``を実行するショートカットです。

フォームの作成は :class:`Symfony¥¥Component¥¥Form¥¥FormBuilder` を介して行われ、そこで各種フィールドの構築や設定を行います。フォームビルダは、フォームファクトリによって作られます。

.. configuration-block::

    .. code-block:: php-standalone

        $form = $formFactory->createBuilder()
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

        echo $twig->render('new.html.twig', array(
            'form' => $form->createView(),
        ));

    .. code-block:: php-symfony

        // src/Acme/TaskBundle/Controller/DefaultController.php
        namespace Acme¥TaskBundle¥Controller;

        use Symfony¥Bundle¥FrameworkBundle¥Controller¥Controller;
        use Symfony¥Component¥HttpFoundation¥Request;

        class DefaultController extends Controller
        {
            public function newAction(Request $request)
            {
                // createFormBuilder is a shortcut to get the "form factory"
                // and then call "createBuilder()" on it
                $form = $this->createFormBuilder()
                    ->add('task', 'text')
                    ->add('dueDate', 'date')
                    ->getForm();

                return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
                    'form' => $form->createView(),
                ));
            }
        }

ご覧のように、フォームの作成はレシピを書くようなものです。新しくフィールドを作成する場合は ``add`` メソッドをコールします。
第1引数にはフィールド名、第2引数にはフィールドの「タイプ」を指定します。Formコンポーネントには多数の :doc:`ビルトインタイプ</reference/forms/types>` が付属されています。

フォームが構築できたので、 :ref:`レンダリング<component-form-intro-rendering-form>` し、
:ref:`送信データを処理<component-form-intro-handling-submission>` する方法を解説します。

デフォルト値の設定
~~~~~~~~~~~~~~~~~~

フォームには（編集フォームなど）デフォルト値が必要なケースがあります。
フォームビルダの作成時にデフォルト値を設定することが可能です。

.. configuration-block::

    .. code-block:: php-standalone

        $defaults = array(
            'dueDate' => new ¥DateTime('tomorrow'),
        );

        $form = $formFactory->createBuilder('form', $defaults)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

    .. code-block:: php-symfony

        $defaults = array(
            'dueDate' => new ¥DateTime('tomorrow'),
        );

        $form = $this->createFormBuilder($defaults)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

.. tip::

    上記の例では、デフォルト値のデータが配列ですが、オブジェクトに直接データをバインドする
    :ref:`data_class<book-forms-data-class>` オプションを使用している場合は、そのオブジェクトのインスタンスがデフォルト値のデータとなります。

.. _component-form-intro-rendering-form:

フォームのレンダリング
~~~~~~~~~~~~~~~~~~~~~~

フォームが完成したので、次はこれをレンダリングしてみましょう。
これは、テンプレートに特殊なフォーム「view」オブジェクトを渡し（上記のコントローラの $form->createView() の部分に注目してください）、フォームヘルパー関数のセットを使用して行われます。

.. code-block:: html+jinja

    <form action="#" method="post" {{ form_enctype(form) }}>
        {{ form_widget(form) }}

        <input type="submit" />
    </form>

.. image:: /images/book/form-simple.png
    :align: center

``form_widget(form)`` はフォームのすべてのフィールドをラベル、（エラーがあれば）エラーメッセージと共にレンダリングします。
これは単純ですが、柔軟性がありません。フォームの見栄えを調整できるように個々のフィールドを一つずつレンダリングしたい所です。
その方法については ":ref:`form-rendering-template`" セクションをご覧ください。

.. _component-form-intro-handling-submission:

送信データの取り扱い
~~~~~~~~~~~~~~~~~~~~

送信データを取り扱うには :method:`Symfony¥¥Component¥¥Form¥¥Form::bind` を使用します。

.. configuration-block::

    .. code-block:: php-standalone

        use Symfony¥HttpFoundation¥Request;
        use Symfony¥Component¥HttpFoundation¥RedirectResponse;

        $form = $formFactory->createBuilder()
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

        $request = Request::createFromGlobals();

        if ($request->isMethod('POST')) {
            $form->bind($request);

            if ($form->isValid()) {
                $data = $form->getData();

                // ... perform some action, such as saving the data to the database

                $response = new RedirectResponse('/task/success');
                $response->prepare($request);

                return $response->send();
            }
        }

        // ...

    .. code-block:: php-symfony

        // ...

        public function newAction(Request $request)
        {
            $form = $this->createFormBuilder()
                ->add('task', 'text')
                ->add('dueDate', 'date')
                ->getForm();

            // only process the form if the request is a POST request
            if ($request->isMethod('POST')) {
                $form->bind($request);

                if ($form->isValid()) {
                    $data = $form->getData();

                    // ... perform some action, such as saving the data to the database

                    return $this->redirect($this->generateUrl('task_success'));
                }
            }

            // ...
        }

これは3つの異なる可能性を含む、一般的なフォームの「ワークフロー」を定義します:

1) 初回のGETリクエスト時（つまり、ユーザがページに初めて訪れる時）、フォームを構築して表示します;

リクエストがPOSTであれば、送信されたデータを処理（\ ``bind`` で）します。その結果:

2) フォームが無効なら、（エラーメッセージと共に）再度フォームを表示します。
3) フォームが有効であれば、適当なアクション（DBへのデータ保存など）を実行し、リダイレクトを行います;

.. note::

    HttpFoundation を使用していない場合、POSTデータをそのまま ``bind`` に渡します::

        if (isset($_POST[$form->getName()])) {
            $form->bind($_POST[$form->getName()]);

            // ...
        }

    ファイルをアップロードしている場合、\ ``bind`` に値を渡す前に、$_FILE配列と$_POST配列をマージする必要があります。

.. _component-form-intro-validation:

バリデーション
~~~~~~~~~~~~~~

各フィールドを構築する際、フォームに検証を追加する最も簡単な方法は、\ ``constraints`` オプションを使用することです:

.. configuration-block::

    .. code-block:: php-standalone

        use Symfony¥Component¥Validator¥Constraints¥NotBlank;
        use Symfony¥Component¥Validator¥Constraints¥Type;

        $form = $formFactory->createBuilder()
            ->add('task', 'text', array(
                'constraints' => new NotBlank(),
            ))
            ->add('dueDate', 'date', array(
                'constraints' => array(
                    new NotBlank(),
                    new Type('¥DateTime'),
                )
            ))
            ->getForm();

    .. code-block:: php-symfony

        use Symfony¥Component¥Validator¥Constraints¥NotBlank;
        use Symfony¥Component¥Validator¥Constraints¥Type;

        $form = $this->createFormBuilder()
            ->add('task', 'text', array(
                'constraints' => new NotBlank(),
            ))
            ->add('dueDate', 'date', array(
                'constraints' => array(
                    new NotBlank(),
                    new Type('¥DateTime'),
                )
            ))
            ->getForm();

フォームがバインドされると、これらのバリデーション制約は自動的に適用され、エラーはエラーのあるフィールドの隣に表示されます。

.. note::

    同梱されているバリデーション制約のすべてのリストについてはこちらをを参照してください。
    :doc:`/reference/constraints`

.. _Packagist: https://packagist.org/packages/symfony/form
.. _Twig:      http://twig.sensiolabs.org
.. _`Twig Configuration`: http://twig.sensiolabs.org/doc/intro.html

.. 2013/10/02 issei-m d6708b06546f3b348aa1be4e0d41e86dd087955e
