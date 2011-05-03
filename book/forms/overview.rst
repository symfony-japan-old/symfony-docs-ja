.. index::
   single: Forms

フォームを扱う
==============

.. 翻訳を更新するまで以下を表示
.. caution::

    このドキュメントの内容は古い内容です。最新の内容は公式の英語ドキュメントをご確認ください。

Symfony2 はビルトインされたフォームコンポーネントを備えています。これにより、HTML フォームを表示したり、レンダリングしたり、送信したりすることができます。

Symfony2 の :class:`Symfony\\Component\\HttpFoundation\\Request` クラス単独で送信したフォームを処理することが可能なだけでなく、フォームコンポーネントは以下のようなフォームに関連した数々の一般的処理の面倒も見ることができます。

1. 自動生成されたフォームフィールドを含んだ HTML フォームの表示
2. 送信されたデータの PHP データ型への変換
3. POPOs (Plain Old PHP Objects) からのデータの読み込み、あるいは POPOs へのデータの書き込み
4. Symfony2 の ``Validator`` を使用した、送信されたデータのバリデーション
5. データ送信の CSRF 攻撃からの保護

概要
----

コンポーネントは以下のコンセプトからなっています。

*フィールド*
  送信データを標準化された値に変換するクラスです。

*フォーム*
  バリデーションをどのように行うのか定義されたフィールドの集まりです。

*テンプレート*
  HTML にフォームやフィールドをレンダリングするファイルです。

*ドメインオブジェクト*
  デフォルト値やどこに送信データが書き込まれたかをフォームが入れるためのオブジェクトです。

フォームコンポーネントの動作が依存しているのは、HttpFoundation と Validator コンポーネントだけです。国際化の機能を使用したい時には、PHP の国際化拡張が必要になります。

フォームオブジェクト
--------------------

フォームオブジェクトは、送信データをあなたのアプリケーションで使われているフォーマットに変換するフィールドの集まりをカプセル化します。フォームクラスは :class:`Symfony\\Component\\Form\\Form` のサブクラスとして生成されます。一連のフィールドを持つフォームを初期化するには、 ``configure()`` メソッドを使用してください。

.. code-block:: php

    // src/Sensio/HelloBundle/Contact/ContactForm.php
    namespace Sensio\HelloBundle\Contact;

    use Symfony\Component\Form\Form;
    use Symfony\Component\Form\TextField;
    use Symfony\Component\Form\TextareaField;
    use Symfony\Component\Form\CheckboxField;

    class ContactForm extends Form
    {
        protected function configure()
        {
            $this->add(new TextField('subject', array(
                'max_length' => 100,
            )));
            $this->add(new TextareaField('message'));
            $this->add(new TextField('sender'));
            $this->add(new CheckboxField('ccmyself', array(
                'required' => false,
            )));
        }
    }

フォームは ``Field`` オブジェクトからなっています。この例の場合、フォームは ``subject`` 、 ``message`` 、 ``sender`` 、 ``ccmyself`` の各フィールドからなっています。 ``TextField`` 、 ``TextareaField`` 、 ``CheckboxField`` は、使用可能なフォームフィールドのうちの3つです。使用可能なフォームフィールドの全リストは、 :doc:`フォームフィールド <fields>` にあります。

コントローラ内でフォームを使用する
----------------------------------

コントローラ内でフォームを使用する際の一般的なパターンは、以下のようになります。

.. code-block:: php

    // src/Sensio/HelloBundle/Controller/HelloController.php
    public function contactAction()
    {
        $contactRequest = new ContactRequest($this->get('mailer'));
        $form = ContactForm::create($this->get('form.context'), 'contact');

        // POST リクエストが送信されたら、送信データを $contactRequest に入れ、
        // オブジェクトのバリデーションを行う
        $form->bind($this->get('request'), $contactRequest);

        // フォームが送信され、内容が有効な場合は...
        if ($form->isValid()) {
            $contactRequest->send();
        }

        // $contactRequest内の値と共にフォームを表示
        return $this->render('HelloBundle:Hello:contact.html.twig', array(
            'form' => $form
        ));
    }

この例には2つのコードパスがあります。

1. フォームが送信されないか有効でなかった場合、単純にテンプレートに移動します。
2. フォームが送信され有効だった場合、コンタクトリクエストが送信されます。

この例では、 ``create()`` staticメソッドでフォームを作成しています。このメソッドは、デフォルトサービス (例えば ``Validator`` ) と、フォームが動作するために必要な設定の全てを含むフォームコンテキストを必要とします。

.. note:

    もし Symfony2 自体あるいは Symfony2 のサービスコンテナを使用しない場合でも心配ありません。 ``FormContext`` と ``Request`` は簡単に手動で作成できます。

    .. code-block:: php

        use Symfony\Component\Form\FormContext;
        use Symfony\Component\HttpFoundation\Request;

        $context = FormContext::buildDefault();
        $request = Request::createFromGlobals();

フォームとドメインオブジェクト
------------------------------

前の例では、 ``ContactRequest`` はフォームに関連づいていました。このオブジェクトのプロパティ値は、フォームフィールドを埋めるのに使われます。バインドの後、送信データの値はオブジェクトに再度書き込まれます。 ``ContactRequest`` クラスは以下のようになっています。

.. code-block:: php

    // src/Sensio/HelloBundle/Contact/ContactRequest.php
    namespace Sensio\HelloBundle\Contact;

    class ContactRequest
    {
        protected $subject = 'Subject...';

        protected $message;

        protected $sender;

        protected $ccmyself = false;

        protected $mailer;

        public function __construct(\Swift_Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function setSubject($subject)
        {
            $this->subject = $subject;
        }

        public function getSubject()
        {
            return $this->subject;
        }

        // 他のプロパティ用のセッタとゲッタ
        // ...

        public function send()
        {
            // メールを送信
            $message = \Swift_Message::newInstance()
                ->setSubject($this->subject)
                ->setFrom($this->sender)
                ->setTo('me@example.com')
                ->setBody($this->message);

            $this->mailer->send($message);
        }
    }

.. note::

    メール送信についての詳細は :doc:`Emails </cookbook/email>` を参照してください。

フォーム内の各フィールドに対して、ドメインオブジェクトのクラスに以下のいずれかが必要です。

1. フィールド名を含むパブリックなプロパティ、または
2. "set" または "get" から始まり、先頭が大文字のフィールド名が続く、パブリックなセッターおよびゲッター

送信データのバリデーション
--------------------------

フォームは、送信されたフォームの値が有効であるかを確認するため、 ``Validator`` コンポーネントを使用します。ドメインオブジェクト上、フォーム上、あるいはフィールド上の全ての制約は、 ``bind()`` が呼び出された時にバリデーションが実行されます。不正なデータが入ったフォームを送信できないことを確実にするために、 ``ContactRequest`` にはいくつかの制約が追加されます。

.. code-block:: php

    // src/Sensio/HelloBundle/Contact/ContactRequest.php
    namespace Sensio\HelloBundle\Contact;

    class ContactRequest
    {
        /**
         * @validation:MaxLength(100)
         * @validation:NotBlank
         */
        protected $subject = 'Subject...';

        /**
         * @validation:NotBlank
         */
        protected $message;

        /**
         * @validation:Email
         * @validation:NotBlank
         */
        protected $sender;

        /**
         * @validation:AssertType("boolean")
         */
        protected $ccmyself = false;

        // コードが続く...
    }

制約を満たさない場合、対応するフォームフィールドの横にエラーが表示されます。詳しくは、 :doc:`バリデーションの制約 </book/validator/constraints>` を参照してください。

フォームフィールドを自動生成する
--------------------------------

Doctrine2 または Symfony の ``Validator`` を使用しているのであれば、Symfony はあなたのドメインクラスについて既にかなりのことを知っていることになります。どのデータタイプがプロパティをデータベース内で永続化するために使われるか、プロパティがどんなバリデーションの制約を持っているか、といったことです。フォームコンポーネントは、どんな設定でどのフィールドタイプが作られるべきかを「推測」するために、これらの情報を使うことができます。

この機能を使用するには、関連するドメインオブジェクトのクラスをフォームが知っている必要があります。このようなクラスは、 ``setDataClass()`` を使用し、クラス名の完全修飾名を文字列として渡すことによって、フォームの ``configure()`` メソッドの中で設定することができます。プロパティ名だけで ``add()`` を呼び出すと、最適なフィールドが自動的に作成されます。

.. code-block:: php

    // src/Sensio/HelloBundle/Contact/ContactForm.php
    class ContactForm extends Form
    {
        protected function configure()
        {
            $this->setDataClass('Sensio\\HelloBundle\\Contact\\ContactRequest');
            $this->add('subject');  // max_lengthが100文字のTextField
                                    // (@MaxLength制約による)
            $this->add('message');  // TextField
            $this->add('sender');   // EmailField (@Email制約による)
            $this->add('ccmyself'); // CheckboxField
                                    // (@AssertType("boolean")制約による)
        }
    }

これらフィールドの推測は、もちろんいつでも正しいとは限りません。 ``message`` というプロパティに対してSymfony が ``TextField`` を作ったとして、バリデーションの制約からはあなたが実は ``TextareaField`` が欲しかったということは分からないのです。従って、このフィールドは手動で作成しなくてはなりません。あるいは、2つ目のパラメータを渡して、フィールド生成のオプションを調整することもできます。長さを制限するために、 ``max_length`` オプションを ``sender`` フィールドに追加できます。

.. code-block:: php

    // src/Sensio/HelloBundle/Contact/ContactForm.php
    class ContactForm extends Form
    {
        protected function configure()
        {
            $this->setDataClass('Sensio\\HelloBundle\\Contact\\ContactRequest');
            $this->add('subject');
            $this->add(new TextareaField('message'));
            $this->add('sender', array('max_length' => 50));
            $this->add('ccmyself');
        }
    }

フォームフィールドの自動生成は、開発速度を上げ、コードの重複を減らすのに役立ちます。クラスプロパティに関する情報を一度保存してしまえば、あとは Symfony2 に他の仕事を任せることができます。

HTML としてフォームをレンダリングする
-------------------------------------

コントローラ内の場合、 ``form`` 変数にフォームを入れてテンプレートに渡しました。テンプレート内の場合は、フォームの生のプロトタイプを出力するため、 ``form_field`` ヘルパーを使用できます。

.. code-block:: html+jinja

    # src/Sensio/HelloBundle/Resources/views/Hello/contact.html.twig
    {% extends 'HelloBundle::layout.html.twig' %}

    {% block content %}
    <form action="#" method="post">
        {{ form_field(form) }}

        <input type="submit" value="Send!" />
    </form>
    {% endblock %}

HTML 出力をカスタマイズする
---------------------------

ほとんどのアプリケーションにおいて、フォームの HTML をカスタマイズしたくなることでしょう。それは、別のビルトインフォームレンダリングヘルパーを使用することによって可能になります。

.. code-block:: html+jinja

    # src/Sensio/HelloBundle/Resources/views/Hello/contact.html.twig
    {% extends 'HelloBundle::layout.html.twig' %}

    {% block content %}
    <form action="#" method="post" {{ form_enctype(form) }}>
        {{ form_errors(form) }}

        {% for field in form %}
            {% if not field.ishidden %}
            <div>
                {{ form_errors(field) }}
                {{ form_label(field) }}
                {{ form_field(field) }}
            </div>
            {% endif %}
        {% endfor %}

        {{ form_hidden(form) }}
        <input type="submit" />
    </form>
    {% endblock %}

Symfony2 には以下のヘルパーが用意されています。

*``form_enctype``*
  フォームタグの ``enctype`` 属性を出力します。ファイルのアップロードのために必須です。

*``form_errors``*
  フィールドまたはフォームのエラーと共に ``<ul>`` タグを出力します。

*``form_label``*
  Outputs the ``<label>`` tag of a field.
  フィールドの ``<label>`` タグを出力します。

*``form_field``*
  フィールドまたはフォームの HTML を出力します。

*``form_hidden``*
  フォームの隠しフィールドを出力します。

フォームのレンダリングに関する詳細は :doc:`テンプレート内でフォームを使用する <view>` を参照してください。

おめでとうございます！ Symfony2 を使って、最初の全機能版フォームを作成できましたね。
